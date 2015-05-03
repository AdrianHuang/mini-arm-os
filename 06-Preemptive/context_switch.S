.syntax unified

.type svc_handler, %function
.global svc_handler
.type systick_handler, %function
.global systick_handler
svc_handler:
systick_handler:
#ifdef GET_CXT_SWITCH_COST
	push {r0, lr}
	mov r0, #0
	bl trace_context_switch
	pop {r0, lr}
#endif
	/* save user state */
	mrs r0, psp
	stmdb r0!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}

	/* load kernel state */
	pop {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}
	msr psr_nzcvq, ip

	bx lr

.global activate
activate:
	/* save kernel state */
	mrs ip, psr
	push {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}

	/* switch to process stack */
	msr psp, r0
	mov r0, #3
	msr control, r0
	isb

	/* load user state */
	pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}

#ifdef GET_CXT_SWITCH_COST
	push {r0, lr}
	mov r0, #1
	bl trace_context_switch
	pop {r0, lr}

#endif
	/* jump to user task */
	bx lr
