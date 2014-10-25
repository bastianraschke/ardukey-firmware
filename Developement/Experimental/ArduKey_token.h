#ifndef __ARDUKEY_TOKEN_H__
#define __ARDUKEY_TOKEN_H__


class ArduKeyToken
{
    public:
        ArduKeyToken();
        ~ArduKeyToken();




        bool getToken(unsigned char token[16]);

    private:
        int _;

};


#endif