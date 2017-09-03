coredumpctl gdb $(coredumpctl list qt-test|tail -n 1 |sed s/.\*EEST//g|sed s/1000.\*//g) 
