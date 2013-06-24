#include <string.h>

#include <litecore.h>
#include "gw3761.h"



//External Functions
void gw3761_Response(p_gw3761 p)
{
    uint8_t *pData, *pEnd;
    int res = 0;
    buf b = {0};
    u_word2 uDu;

    switch (p->rmsg.afn) {
    case GW3761_AFN_CONFIRM:
    case GW3761_AFN_LINKCHECK:
        //�����Ӧ
        break;
    case 0x01:  //��λ
        gw3761_ResponseReset(p);
        break;
    case 0x04:  //���ò���
        gw3761_ResponseSetParam(p);
        break;
    case 0x0C:  //һ������
        gw3761_ResponseData1(p);
        break;
    case 0x0D:  //��������
        gw3761_ResponseData2(p);
        break;
    default:
        //ͳһ��Ӧ
        pData = p->rmsg.data->p;
        pEnd = pData + p->rmsg.data->len;
        for (; (pData + 4) <= pEnd; ) {
            memcpy(&uDu, pData, 4);
            buf_Push(b, pData, 4);
            pData += 4;
            switch (p->rmsg.afn) {
            case 0x03:  //�м�����
                break;
            case 0x05:  //��������
                res += gw3761_ResponseCtrlCmd(p, &uDu, &pData);
                break;
#if GW3761_ESAM_ENABLE
            case 0x06:  //��Ϣ��֤
                res += gw3761_ResponseAuthority(p, b, &uDu, &pData);
                break;
#endif
            case 0x09:  //��ȡ����
                res += gw3761_ResponseGetConfig(p, b, &uDu);
                break;
            case 0x0A:  //��ȡ����
                res += gw3761_ResponseGetParam(p, b, &uDu, &pData);
                break;
            case 0x0E:  //���¼�
                res += gw3761_ResponseData3(p, b, &uDu, &pData);
                break;
            case 0x0F:  //�ļ�����
                res += gw3761_ResponseFileTrans(p, b, &uDu, &pData);
                break;
            case 0x10:  //����ת��
                res += gw3761_ResponseTransmit(p, b, &uDu, &pData);
                break;
            default:
                buf_Unpush(b, 4);
                break;
            }
        }
        if (b->len > 4) {
            gw3761_TmsgSend(p, GW3761_FUN_RESPONSE, p->rmsg.afn, b, DLRCP_TMSG_RESPOND);
        } else {
            if (res)
                gw3761_TmsgConfirm(p);
            else
                gw3761_TmsgReject(p);
        }
        buf_Release(b);
        break;
    }
}


