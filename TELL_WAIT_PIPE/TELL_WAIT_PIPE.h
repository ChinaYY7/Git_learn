#ifndef _TELL_WAIT_PIPE_H
#define _TELL_WAIT_PIPE_H
void TELL_WAIT(void);
void Tell_parent(pid_t pid);
void Wait_child(void);
void Tell_child(pid_t pid);
void Wait_parent(void);

#endif