from typesafe_sdk import Choice, TypeSafeClient


report = input("作業報告: ").strip()
if not report:
	raise SystemExit

client = TypeSafeClient()
response = client.system_one(
	state=report,
	questions={
		"action": Choice(
            instructions="作業報告を読んで、次に取るべき対応を選んでください",
			criteria={
				"CONTINUE": "異常がないことが明確で、安全に作業を続けられる",
				"CHECK": "異常の疑いがある、または情報が不足しており、状況の確認が必要",
				"STOP": "煙、火災、負傷、重大な故障など明確な危険があり、直ちに停止が必要",
			},
		)
	},
)

answer = response.answers["action"]
print(f"対応: {answer.choice}")
print(f"確率: {answer.probabilities}")
