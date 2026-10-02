import os
from collections import Counter

from typesafe_sdk import Noul, TypeSafeClient


def classify(probability: float) -> str:
    if probability >= 0.7:
        return "ACTION"
    if probability <= 0.3:
        return "NO_ACTION"
    return "REVIEW"


def main() -> None:
    if not os.environ.get("TYPESAFE_API_KEY"):
        print("環境変数 TYPESAFE_API_KEY を設定してください。")
        return

    memos = [
        "明日までに参加申込フォームを提出してください。",
        "参加申込フォームは提出済みです。追加対応は不要です。",
        "来週の打ち合わせまでに資料を確認しておいてください。",
        "今日の打ち合わせでは、新しい開発環境が紹介されました。",
    ]

    questions = {
        "needs_action": Noul(
            instructions=(
                "このメモは、読み手に未完了の対応を求めていますか？"
                "期限の記載だけではなく、対応済みかどうかも考慮してください。"
                "提出済み、完了済み、追加対応不要の場合や、"
                "単なる出来事の報告の場合は「いいえ」と判断してください。"
            )
        )
    }

    counts = Counter()

    # SDKが環境変数 TYPESAFE_API_KEY を読み取ります。
    with TypeSafeClient() as client:
        for number, memo in enumerate(memos, start=1):
            response = client.system_one(
                state=memo,
                questions=questions,
            )

            probability = response.nouls["needs_action"].noul
            category = classify(probability)
            counts[category] += 1

            print(f"{number}: {category} ({probability:.3f})")

    print()
    for category in ("ACTION", "REVIEW", "NO_ACTION"):
        print(f"{category}: {counts[category]}")


if __name__ == "__main__":
    main()