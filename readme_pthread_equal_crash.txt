Thread 1 "kirikiroid2" received signal SIGSEGV, Segmentation fault.
0x0000555555d4deb2 in TJS::tTJSCriticalSectionImpl::lock (this=0x555556a4ec20)
    at src/core/tjs2/tjsUtils.cpp:43
43		if (_tid != NULL && pthread_equal(*_tid, *id)) return false;
(gdb) p pthread_equal
$1 = {int (pthread_t, pthread_t)} 0x7ffff72a3af0 <__pthread_equal>
(gdb) p id
$2 = (pthread_t *) 0x55555dec8380
(gdb) p *_tid
Cannot access memory at address 0x0
(gdb) p _tid
$3 = (pthread_t *) 0x0
(gdb) p _tid != NULL
No symbol "__null" in current context.
(gdb) p _tid != 0
$4 = false
(gdb) 

---

bool tTJSCriticalSectionImpl::lock() {
	pthread_t *id = new pthread_t();
	*id = pthread_self();
crash here--->	if (_tid != NULL && pthread_equal(*_tid, *id)) return false;
	pthread_mutex_lock(&_mutex);
	_tid = id;
	return true;
}

--------






