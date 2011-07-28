#
# Regular cron jobs for the vibe2modules package
#
0 4	* * *	root	[ -x /usr/bin/vibe2modules_maintenance ] && /usr/bin/vibe2modules_maintenance
