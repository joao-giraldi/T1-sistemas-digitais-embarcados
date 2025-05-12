#ifndef SCHEDULER_H
#define	SCHEDULER_H

#include "types.h"
void __reentrant scheduler(void);
void __reentrant rr_scheduler(void);
void __reentrant priority_scheduler(void);

#endif	/* SCHEDULER_H */

