from textual.app import App, ComposeResult
from textual.containers import Container, Horizontal, Vertical
from textual.widgets import Button, Static, Header
from textual.binding import Binding
import random

class RockPaperScissorsApp(App):
    """Textualを使ったじゃんけんアプリ"""
    
    CSS = """
    Screen {
        align: center middle;
    }
    
    #title {
        width: 100%;
        text-align: center;
        padding: 1;
        background: $boost;
        color: $text;
        text-style: bold;
    }
    
    #buttons {
        width: auto;
        height: auto;
        padding: 1;
        align: center middle;
    }
    
    Button {
        margin: 0 1;
        min-width: 15;
    }
    
    #result {
        width: 100%;
        height: auto;
        padding: 1;
        margin-top: 1;
        text-align: center;
        border: solid green;
    }
    
    .win {
        color: green;
        text-style: bold;
    }
    
    .lose {
        color: red;
        text-style: bold;
    }
    
    .draw {
        color: yellow;
        text-style: bold;
    }
    """
    
    BINDINGS = [
        Binding("1", "choose('グー')", "グー"),
        Binding("2", "choose('チョキ')", "チョキ"),
        Binding("3", "choose('パー')", "パー"),
        Binding("q", "quit", "終了"),
    ]
    
    def __init__(self):
        super().__init__()
        self.hands = ["グー", "チョキ", "パー"]
    
    def compose(self) -> ComposeResult:
        """画面を構成する"""
        yield Static("🎮 じゃんけんアプリ 🎮", id="title")
        
        with Vertical():
            with Horizontal(id="buttons"):
                yield Button("✊ グー", id="rock", variant="primary")
                yield Button("✌️ チョキ", id="scissors", variant="success")
                yield Button("✋ パー", id="paper", variant="warning")
            
            yield Static("ボタンを押してじゃんけんを始めましょう！\n(キーボード: 1=グー, 2=チョキ, 3=パー)", id="result")
    
    def on_button_pressed(self, event: Button.Pressed) -> None:
        """ボタンが押されたときの処理"""
        button_id = event.button.id
        
        if button_id == "rock":
            self.play_game("グー")
        elif button_id == "scissors":
            self.play_game("チョキ")
        elif button_id == "paper":
            self.play_game("パー")
    
    def action_choose(self, hand: str) -> None:
        """キーボード操作での選択"""
        self.play_game(hand)
    
    def play_game(self, user_hand: str) -> None:
        """じゃんけんのゲームロジック"""
        # コンピュータの手をランダムに選択
        computer_hand = random.choice(self.hands)
        
        # 勝敗判定
        result, css_class = self.judge(user_hand, computer_hand)
        
        # 結果表示を更新
        result_widget = self.query_one("#result", Static)
        result_text = f"あなたの手：{user_hand}\nコンピュータ：{computer_hand}\n結果：{result}"
        result_widget.update(result_text)
        
        # CSSクラスを更新して色を変更
        result_widget.remove_class("win", "lose", "draw")
        result_widget.add_class(css_class)
    
    def judge(self, user: str, computer: str) -> tuple[str, str]:
        """勝敗を判定"""
        if user == computer:
            return "あいこ", "draw"
        
        win_conditions = {
            "グー": "チョキ",
            "チョキ": "パー",
            "パー": "グー"
        }
        
        if win_conditions[user] == computer:
            return "あなたの勝ち！", "win"
        else:
            return "あなたの負け...", "lose"


if __name__ == "__main__":
    app = RockPaperScissorsApp()
    app.run()