#include<termios.h>

#include<unistd.h>

#include<stdlib.h>

#include<ctype.h>

#include<stdio.h>

struct termios orig_termios;

void disableRawMode(){
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&orig_termios);
}
void enableRawMode(){
    tcgetattr(STDIN_FILENO,&orig_termios);//获取当前标准输入模式

    atexit(disableRawMode);//使退出程序时，终端禁用生模式

    struct termios raw=orig_termios;
    //禁用IXON会关闭software flow control而禁用ICRNL会关闭将输入的回车符转换成换行符
    raw.c_iflag&=~(IXON|ICRNL);
    //echo关掉后自己输入会不会显示；ICANON关掉后会取消行缓冲;ISIG关掉后会禁用符号字符的功能
    //IEXTEN关掉后会禁用掉输入处理的扩展功能
    raw.c_lflag &=~(ECHO|ICANON|ISIG|IEXTEN);

    tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw);
}
int main(){
    char c;

    enableRawMode();

    while(read(STDIN_FILENO,&c,1)==1&&c!='q'){

        if(iscntrl(c)){
            printf("%d\n",c);
        }
        else{
            printf("%d(%c)\n",c,c);
        }

    }

    return 0;
}