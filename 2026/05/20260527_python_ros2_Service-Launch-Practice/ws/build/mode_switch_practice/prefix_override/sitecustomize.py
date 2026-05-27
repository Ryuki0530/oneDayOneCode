import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/als0079/Desktop/oneDayOneCode/2026/05/20260527_python_ros2_Service-Launch-Practice/ws/install/mode_switch_practice'
