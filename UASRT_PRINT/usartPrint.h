#ifndef __USART_PRINT
    #define __USART_PRINT

    void USART_PrintStr(char *);
    void USART_PrintNum(int, char *);
    void USART_PrintStrWithEnding(char *, char *);
    void USART_PrintCallbackStr(char *);
    char *Int_To_String(int, char *);
    void USART_SendChar(char);

#endif