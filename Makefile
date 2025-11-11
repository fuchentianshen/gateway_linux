
log:=thirdparty/log.c thirdparty/log.h

# -I 给gcc添加头文件搜索目录
# $^ 表示所以的依赖项
# $@ 表示目标名
log_test:test/log_test.c $(log)
	-gcc $^ -o $@ -I thirdparty
	-./$@
	-rm $@
