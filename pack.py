import os

# 定义你想要打包的文件后缀
VALID_EXTENSIONS = {'.cpp', '.h', '.hpp', '.txt'} # 包含 CMakeLists.txt
# 定义需要忽略的文件夹
IGNORE_DIRS = {'.git', 'old', 'build', 'cmake-build-debug'}

output_file_name = "project_code.txt"

with open(output_file_name, "w", encoding="utf-8") as outfile:
    for root, dirs, files in os.walk("."):
        # 排除忽略目录
        dirs[:] = [d for dirs in [dirs] for d in dirs if d not in IGNORE_DIRS]
        
        for file in files:
            # 只打包 C++ 代码和 CMake
            if any(file.endswith(ext) for ext in VALID_EXTENSIONS) and file != output_file_name and file != "pack_project.py":
                file_path = os.path.join(root, file)
                outfile.write(f"\n\n{'='*40}\n")
                outfile.write(f"FILE: {file_path}\n")
                outfile.write(f"{'='*40}\n\n")
                try:
                    with open(file_path, "r", encoding="utf-8") as infile:
                        outfile.write(infile.read())
                except Exception as e:
                    outfile.write(f"// Error reading file: {str(e)}")

print(f"🎉 打包完成！已生成: {output_file_name}，快把它拖拽上传到聊天框吧！")
