from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import subprocess

from textual.app import App, ComposeResult
from textual.binding import Binding
from textual.containers import Horizontal, Vertical
from textual.screen import ModalScreen
from textual.widgets import Button, Footer, Header, Input, Label, ListItem, ListView, Static


class GitCommandError(RuntimeError):
    """Raised when a git command returns a non-zero exit code."""


@dataclass
class BranchInfo:
    name: str
    is_current: bool
    commit_short: str
    subject: str


class GitService:
    def __init__(self, repo_path: Path) -> None:
        self.repo_path = repo_path

    def _run(self, *args: str) -> str:
        result = subprocess.run(
            ["git", *args],
            cwd=self.repo_path,
            capture_output=True,
            text=True,
            check=False,
        )
        if result.returncode != 0:
            message = result.stderr.strip() or result.stdout.strip() or "Unknown git error"
            raise GitCommandError(message)
        return result.stdout

    def is_git_repo(self) -> bool:
        try:
            output = self._run("rev-parse", "--is-inside-work-tree").strip().lower()
            return output == "true"
        except GitCommandError:
            return False

    def list_branches(self) -> list[BranchInfo]:
        output = self._run(
            "for-each-ref",
            "--format=%(refname:short)\t%(HEAD)\t%(objectname:short)\t%(subject)",
            "refs/heads",
        )

        branches: list[BranchInfo] = []
        for raw in output.splitlines():
            if not raw.strip():
                continue
            name, head, commit_short, subject = raw.split("\t", 3)
            branches.append(
                BranchInfo(
                    name=name.strip(),
                    is_current=head.strip() == "*",
                    commit_short=commit_short.strip(),
                    subject=subject.strip(),
                )
            )
        return branches

    def validate_branch_name(self, name: str) -> bool:
        try:
            self._run("check-ref-format", "--branch", name)
            return True
        except GitCommandError:
            return False

    def checkout_branch(self, name: str) -> None:
        self._run("switch", name)

    def create_branch(self, name: str) -> None:
        self._run("switch", "-c", name)

    def delete_branch(self, name: str) -> None:
        self._run("branch", "-d", name)


class BranchListItem(ListItem):
    def __init__(self, branch: BranchInfo) -> None:
        self.branch = branch
        marker = "*" if branch.is_current else " "
        label = Label(f"{marker} {branch.name}")
        super().__init__(label)


class NewBranchScreen(ModalScreen[str | None]):
    CSS = """
    NewBranchScreen {
        align: center middle;
    }

    #new-branch-dialog {
        width: 70;
        max-width: 90%;
        height: auto;
        border: round $accent;
        background: $surface;
        padding: 1 2;
    }

    #new-branch-buttons {
        height: auto;
        align-horizontal: right;
        margin-top: 1;
    }

    #new-branch-error {
        color: $error;
        margin-top: 1;
    }
    """

    def compose(self) -> ComposeResult:
        with Vertical(id="new-branch-dialog"):
            yield Label("New branch name")
            yield Input(placeholder="feature/my-task", id="new-branch-input")
            yield Static("", id="new-branch-error")
            with Horizontal(id="new-branch-buttons"):
                yield Button("Create", id="confirm-new-branch", variant="primary")
                yield Button("Cancel", id="cancel-new-branch")

    def on_mount(self) -> None:
        self.query_one("#new-branch-input", Input).focus()

    def on_input_submitted(self, event: Input.Submitted) -> None:
        if event.input.id == "new-branch-input":
            self.dismiss(event.value)

    def on_button_pressed(self, event: Button.Pressed) -> None:
        if event.button.id == "confirm-new-branch":
            name = self.query_one("#new-branch-input", Input).value
            self.dismiss(name)
            return
        self.dismiss(None)


class ConfirmDeleteScreen(ModalScreen[bool]):
    CSS = """
    ConfirmDeleteScreen {
        align: center middle;
    }

    #confirm-delete-dialog {
        width: 70;
        max-width: 90%;
        height: auto;
        border: round $error;
        background: $surface;
        padding: 1 2;
    }

    #confirm-delete-buttons {
        height: auto;
        align-horizontal: right;
        margin-top: 1;
    }
    """

    def __init__(self, branch_name: str) -> None:
        super().__init__()
        self.branch_name = branch_name

    def compose(self) -> ComposeResult:
        with Vertical(id="confirm-delete-dialog"):
            yield Label(f"Delete branch '{self.branch_name}'?")
            yield Label("This uses git branch -d (safe delete).")
            with Horizontal(id="confirm-delete-buttons"):
                yield Button("Delete", id="confirm-delete", variant="error")
                yield Button("Cancel", id="cancel-delete")

    def on_button_pressed(self, event: Button.Pressed) -> None:
        self.dismiss(event.button.id == "confirm-delete")


class BranchManagerApp(App[None]):
    CSS_PATH = "styles.tcss"
    BINDINGS = [
        Binding("r", "refresh", "Refresh"),
        Binding("c", "checkout", "Checkout"),
        Binding("n", "new_branch", "New"),
        Binding("d", "delete_branch", "Delete"),
        Binding("q", "quit", "Quit"),
    ]

    def __init__(self) -> None:
        super().__init__()
        self.git = GitService(Path.cwd())
        self.branches: list[BranchInfo] = []
        self.selected_branch_name: str | None = None
        self.repo_available = False

    def compose(self) -> ComposeResult:
        yield Header(show_clock=True)
        with Horizontal(id="main"):
            with Vertical(id="left-pane"):
                yield Label("Local Branches", classes="pane-title")
                yield ListView(id="branch-list")
            with Vertical(id="right-pane"):
                yield Label("Branch Details", classes="pane-title")
                yield Static("No branch selected.", id="detail")
                yield Label("Actions", classes="pane-title")
                with Horizontal(id="actions"):
                    yield Button("Checkout", id="btn-checkout", variant="primary")
                    yield Button("Delete", id="btn-delete", variant="error")
                    yield Button("New Branch", id="btn-new")
                    yield Button("Refresh", id="btn-refresh")
        yield Static(
            "Hint: Arrow keys=select | c=Checkout | n=New | d=Delete | r=Refresh | q=Quit",
            id="hint",
        )
        yield Static("Ready", id="status")
        yield Footer()

    def on_mount(self) -> None:
        self.reload_branches()

    def action_refresh(self) -> None:
        self.reload_branches()

    def action_checkout(self) -> None:
        self.checkout_selected_branch()

    def action_new_branch(self) -> None:
        self.open_new_branch_dialog()

    def action_delete_branch(self) -> None:
        self.confirm_delete_selected_branch()

    def set_status(self, message: str, error: bool = False) -> None:
        status = self.query_one("#status", Static)
        status.update(message)
        status.set_class(error, "error")

    def current_selected_branch(self) -> BranchInfo | None:
        if not self.selected_branch_name:
            return None
        for branch in self.branches:
            if branch.name == self.selected_branch_name:
                return branch
        return None

    def update_detail(self) -> None:
        detail = self.query_one("#detail", Static)
        if not self.repo_available:
            detail.update("Current directory is not a Git repository.")
            return

        selected = self.current_selected_branch()
        if not selected:
            detail.update("No branch selected.")
            return

        lines = [
            f"Name: {selected.name}",
            f"Current: {'Yes' if selected.is_current else 'No'}",
            f"Latest commit: {selected.commit_short or '-'}",
            f"Summary: {selected.subject or '-'}",
        ]
        detail.update("\n".join(lines))

    def set_controls_enabled(self, enabled: bool) -> None:
        self.query_one("#branch-list", ListView).disabled = not enabled
        self.query_one("#btn-checkout", Button).disabled = not enabled
        self.query_one("#btn-delete", Button).disabled = not enabled
        self.query_one("#btn-new", Button).disabled = not enabled

    def reload_branches(self) -> None:
        branch_list = self.query_one("#branch-list", ListView)

        self.repo_available = self.git.is_git_repo()
        if not self.repo_available:
            self.branches = []
            self.selected_branch_name = None
            branch_list.clear()
            self.set_controls_enabled(False)
            self.update_detail()
            self.set_status("Not a Git repository in current directory.", error=True)
            return

        self.set_controls_enabled(True)

        try:
            self.branches = self.git.list_branches()
        except GitCommandError as exc:
            self.set_status(f"Failed to load branches: {exc}", error=True)
            return

        branch_list.clear()
        for branch in self.branches:
            branch_list.append(BranchListItem(branch))

        if self.branches:
            selected_index = 0
            for i, branch in enumerate(self.branches):
                if branch.is_current:
                    selected_index = i
                    break
            branch_list.index = selected_index
            self.selected_branch_name = self.branches[selected_index].name
        else:
            self.selected_branch_name = None

        self.update_detail()
        self.set_status("Branch list updated.")

    def on_list_view_selected(self, event: ListView.Selected) -> None:
        if isinstance(event.item, BranchListItem):
            self.selected_branch_name = event.item.branch.name
            self.update_detail()

    def on_button_pressed(self, event: Button.Pressed) -> None:
        button_id = event.button.id
        if button_id == "btn-refresh":
            self.reload_branches()
        elif button_id == "btn-checkout":
            self.checkout_selected_branch()
        elif button_id == "btn-new":
            self.open_new_branch_dialog()
        elif button_id == "btn-delete":
            self.confirm_delete_selected_branch()

    def checkout_selected_branch(self) -> None:
        if not self.repo_available:
            self.set_status("Operation disabled: not a Git repository.", error=True)
            return

        branch = self.current_selected_branch()
        if not branch:
            self.set_status("No branch selected.", error=True)
            return
        if branch.is_current:
            self.set_status(f"Already on '{branch.name}'.")
            return

        try:
            self.git.checkout_branch(branch.name)
        except GitCommandError as exc:
            self.set_status(f"Checkout failed: {exc}", error=True)
            return

        self.reload_branches()
        self.set_status(f"Checked out '{branch.name}'.")

    def open_new_branch_dialog(self) -> None:
        if not self.repo_available:
            self.set_status("Operation disabled: not a Git repository.", error=True)
            return

        self.push_screen(NewBranchScreen(), self.handle_new_branch_result)

    def handle_new_branch_result(self, branch_name: str | None) -> None:
        if branch_name is None:
            self.set_status("New branch creation canceled.")
            return

        normalized = branch_name.strip()
        if not normalized:
            self.set_status("Branch name must not be empty.", error=True)
            return

        if not self.git.validate_branch_name(normalized):
            self.set_status("Invalid branch name.", error=True)
            return

        if any(branch.name == normalized for branch in self.branches):
            self.set_status(f"Branch '{normalized}' already exists.", error=True)
            return

        try:
            self.git.create_branch(normalized)
        except GitCommandError as exc:
            self.set_status(f"Create failed: {exc}", error=True)
            return

        self.reload_branches()
        self.set_status(f"Created and switched to '{normalized}'.")

    def confirm_delete_selected_branch(self) -> None:
        if not self.repo_available:
            self.set_status("Operation disabled: not a Git repository.", error=True)
            return

        branch = self.current_selected_branch()
        if not branch:
            self.set_status("No branch selected.", error=True)
            return

        if branch.is_current:
            self.set_status("Cannot delete current checked-out branch.", error=True)
            return

        self.push_screen(ConfirmDeleteScreen(branch.name), self.handle_delete_confirm_result)

    def handle_delete_confirm_result(self, confirmed: bool) -> None:
        if not confirmed:
            self.set_status("Delete canceled.")
            return

        branch = self.current_selected_branch()
        if not branch:
            self.set_status("No branch selected.", error=True)
            return

        try:
            self.git.delete_branch(branch.name)
        except GitCommandError as exc:
            self.set_status(f"Delete failed: {exc}", error=True)
            return

        self.reload_branches()
        self.set_status(f"Deleted branch '{branch.name}'.")


if __name__ == "__main__":
    BranchManagerApp().run()
