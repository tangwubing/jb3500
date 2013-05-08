#if 0

//����õ�ʱ�����Ҫ����������Ȼ������
#define PDEBUG(fmt, args...) fprintf(stderr, fmt, args##)

//����ɴ��ɲ�������
#define LOGSTRINGS(fm, ...) printf(fm, ##__VA_ARGS__)
#endif

//External Consts
const char dbg_header[2] = {'\r', '\n'};



//External Functions
void dbg_printf(const char *fmt, ...)
{
	va_list args;
	char str[DBG_BUF_SIZE];

	va_start(args, fmt);
	vsnprintf(str, sizeof(str), fmt, args);
	va_end(args);

	dbg_trace(str);
}


