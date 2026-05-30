# RobotStatus.msg
# ロボットの状態を表すメッセージ

string robot_name
int32 battery_percent
float32 temperature
bool is_emergency

int32 NORMAL = 0
int32 WARNING = 1
int32 ERROR = 2
int32 status_level