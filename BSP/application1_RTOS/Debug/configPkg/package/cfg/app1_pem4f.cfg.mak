# invoke SourceDir generated makefile for app1.pem4f
app1.pem4f: .libraries,app1.pem4f
.libraries,app1.pem4f: package/cfg/app1_pem4f.xdl
	$(MAKE) -f D:\TREC-EXO\Test-Stand-Electronics\BSP\application1_RTOS/src/makefile.libs

clean::
	$(MAKE) -f D:\TREC-EXO\Test-Stand-Electronics\BSP\application1_RTOS/src/makefile.libs clean

