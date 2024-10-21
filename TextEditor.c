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
    //禁用IXON会关闭software flow control而禁用ICRNL会停止将输入的回车符转换成换行符
    raw.c_iflag&=~(IXON|ICRNL);
    //禁用OPOST会关闭输出处理，\n不会被转换成\r\n;回车是将光标移动到行首
    raw.c_oflag&=~OPOST;
    //echo关掉后自己输入会不会显示；ICANON关掉后会取消行缓冲;ISIG关掉后会禁用符号字符的功能如Ctrl-C和Ctrl-Z
    //IEXTEN关掉后会禁用掉输入处理的扩展功能如Ctrl-V
    raw.c_lflag &=~(ECHO|ICANON|ISIG|IEXTEN);
    //禁用INPCK会关闭输入奇偶校验；禁用ISTRIP会关闭将输入的字符去掉最高位
    //BRKINT会关闭break条件；IGNPAR会忽略校验错误的字节；PARMRK会将奇偶校验错误的字节前面加上\377\0
    raw.c_iflag&=~(BRKINT|ICRNL|INPCK|ISTRIP|IXON);//要么是已经关闭的，要么是现代终端不再需要的
    //CS8会设置字符大小为8位
    raw.c_cflag|=(CS8);//在现代终端中默认就是8位

    raw.c_cc[VMIN]=0;
    raw.c_cc[VTIME]=1;

    tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw);
}
int main(){
    char c;

    enableRawMode();

    while(1){
        c='\0';
        read(STDIN_FILENO,&c,1);
        if(iscntrl(c)){
            printf("%d\r\n",c);
        }
        else{
            printf("%d(%c)\r\n",c,c);
        }
        if(c=='q') break;

    }

    return 0;
}