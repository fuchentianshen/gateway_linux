
log:=thirdparty/log.c thirdparty/log.h
json:=thirdparty/cJSON.c thirdparty/cJSON.h

# -g 开启gdb的debug调试（生成的二进制文件中包含调试信息）
# -O0 禁用优化（确保调试一致性）
# -Wall 显示所有警告
CFLAGS := -g -O0 -Wall



# -I 给gcc添加头文件搜索目录
# $^ 表示所以的依赖项
# $@ 表示目标名
log_test:test/log_test.c $(log)
	-gcc $(CFLAGS) $^ -o $@ -I thirdparty
	-./$@
	-rm $@

json_test:test/json_test.c $(json) $(log)
	-gcc $^ -o $@ -I thirdparty
	-./$@
	-rm $@