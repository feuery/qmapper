coredumpctl gdb $(coredumpctl list qt-test|tail -n 1 |sed s/.\*T//g|sed s/1000.\*//g) 
