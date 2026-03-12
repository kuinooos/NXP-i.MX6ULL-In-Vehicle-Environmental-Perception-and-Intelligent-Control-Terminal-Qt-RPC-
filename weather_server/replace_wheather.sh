#!/bin/bash

# 定义要修改的文件名
FILE="wheather.cpp"  # 请将 "your_file.cpp" 替换为你的文件名

# 使用 sed 替换所有 "wheather" 为 "weather"
sed -i 's/wheather/weather/g' "$FILE"

echo "替换完成！"

