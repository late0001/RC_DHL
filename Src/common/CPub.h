
//�ַ�����ֺ������
int getSplitLen(LPCTSTR s1, char c1)
{
	int len=0;
	int i=0;
	int j=strlen(s1);
	int k=0;			//char index
	for (i=0;i<j;i++)
	{
		if(s1[i] == c1)
		{
			len++;
			k=i;
		}
	}
	if(k+1<j) len++;
	return len;
}

//��ȡ��ֺ���ƶ��ַ��� s1ԭʼ�ַ��� c1�ָ��ַ� s2�����ַ���
void getSplit(LPCTSTR s1, char c1, int i_index, char* s2)
{
	int len=0;
	int i=0;
	int j=strlen(s1);
	int k=0;			//char index
	
	for (i=0;i<j;i++)
	{
		if(s1[i] == c1)
		{
			len++;
			continue;
		}
		
		if (len == i_index)
		{
			s2[k] = s1[i];
			k++;
		}
		if (len > i_index)
			break;
	}
	s2[k] = 0;
}
