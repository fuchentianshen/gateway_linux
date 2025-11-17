
log:=thirdparty/log.c thirdparty/log.h
json:=thirdparty/cJSON.c thirdparty/cJSON.h
app_common:=app/app_common.c app/app_common.h
app_message:=app/app_message.c app/app_message.h
app_mqtt:=app/app_mqtt.c app/app_mqtt.h
app_pool:=app/app_pool.c app/app_pool.h
app_buffer:=app/app_buffer.c app/app_buffer.h
app_modbus:=app/app_modbus.c app/app_modbus.h
ota_http:=ota/ota_http.c ota/ota_http.h
app_device:=app/app_device.c app/app_device.h

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

app_common_test:test/app_common_test.c $(app_common) $(log)
	-gcc $^ -o $@ -I app -I thirdparty
	-./$@
	-rm $@

app_message_test:test/app_message_test.c $(app_message) $(log) $(json)
	-gcc $^ -o $@ -I app -I thirdparty
	-./$@
	-rm $@


mqtt_test: test/mqtt_test.c
	-$(CC) $^ -o $@ -lpaho-mqtt3c
	-./$@
	-rm $@

app_mqtt_test: test/app_mqtt_test.c $(app_mqtt) $(log) 
	-gcc $^ -o $@ -I app -I thirdparty -lpaho-mqtt3c
	-./$@
	-rm $@

app_pool_test: test/app_pool_test.c $(app_pool) $(log)
	-gcc $^ -o $@ -I app -I thirdparty
	-./$@
	-rm $@

app_buffer_test: test/app_buffer_test.c $(app_buffer) $(log)
	-gcc $^ -o $@ -I app -I thirdparty
	-./$@
	-rm $@

app_modbus_test: test/app_modbus_test.c $(app_modbus) $(log)
	-gcc $^ -o $@ -I app -I thirdparty -l modbus
	-./$@
	-rm $@


device_objs:=$(app_device) $(log) $(json) $(app_buffer)\
	$(app_modbus) $(app_pool) $(app_mqtt) $(app_message) 
app_device_test: test/app_device_test.c $(device_objs)
	-$(CC)  $^ -o $@ -Ithirdparty -Iapp -lmodbus -lpaho-mqtt3c
	./$@
	-rm $@


ota_http_test: test/ota_http_test.c $(ota_http) $(log)
	-gcc $^ -o $@ -I ota -I thirdparty -l curl
	-./$@
	-rm $@