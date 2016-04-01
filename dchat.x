/*
An External Data Representation (XDR) file describing the protocol
definition for the qChat program. ONC RPCGEN is being used as the
protocol compiler for this project.

*/

const MAX_MSG_LEN = 512;        /* 64 Bytes */
const MAX_USR_LEN = 32;         /* 4 Bytes  */
const MAX_IP_LEN =  32;		    /* 4 Bytes  */
const BUFLEN = 556;             /* WTF MATE */

typedef string msg_send<MAX_MSG_LEN>;
typedef string uname<MAX_USR_LEN>;
typedef string hoststr<MAX_IP_LEN>;

enum msg_type_t {TEXT = 0, NEWUSER = 1, USEREXIT = 2, ELECTION = 3};
enum status_code {JSUCCESS = 0, JFAILURE = 1, UNAMEINUSE = 2, UNAMEINVALID = 3};


struct cname {

uname userName;
hoststr hostname;
int lport;
int leader_flag;

};

typedef struct cname cname;

struct clist {
cname clientlist<>;

};

struct msg_recv {
msg_send msg_sent;
uname user_sent;
unsigned int seq_num;
msg_type_t msg_type;
};

program DCHAT {
version DCHATVERS {

int JOIN(cname) = 1;
int SEND(msg_recv) = 2;
int EXIT(uname) = 3;
msg_recv REDELIVER(unsigned int) = 4;
unsigned int HEARTBEAT(unsigned int) = 5;
void SHUTDOWNSERV() = 6;

} = 1;
} = 0x20000001;
