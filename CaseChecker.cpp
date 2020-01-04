#include "stdafx.h"
#include "CaseChecker.h"
#include <string.h>


CCaseChecker::~CCaseChecker()
{	
	DeleteAllNode();
}


// 가능 한 경우 따져 보기(기존 Check 표시를 기반으로 Value의 갯수를 분배하기) 
int CCaseChecker::IntDivideFuc(int *Value,  int nBuket, int mBall, int nThBuket = 0)
{
	// 정수분할,  n개 버켓에 m개볼.	
	if (nThBuket >= nBuket-1)
	{
		Value[nThBuket] = mBall;
		// 중복없이 저장하면 된다.
		// 여기서 조건 Check가 들어가면됨,
		char StrBuffer[180];
		int FLSpace;
		int ValueSpace;
		int ValueSum;
		Node<CFlagNode> *pFlCur = mpFl;
		Node<int> *pValue = mpValueHeader;
		_strnset(StrBuffer, 0, 80);
		strcat(StrBuffer, ";");
		bool pass = false;
		for (int k = 0; k < nBuket ; k++)
		{
			FLSpace = pFlCur->mValue.mEnd - pFlCur->mValue.mStart + 1;
			ValueSum = 0;
			ValueSpace = 0;
			for (int v = 0; v < Value[k]; v++)
			{
				//Value space < FL space

				ValueSum += pValue->mValue;
				ValueSpace += (ValueSpace) ? pValue->mValue + 1 : pValue->mValue;
				strcat(StrBuffer, ",");
				
				pValue = pValue->pNext;				
			} // for v

			if (ValueSpace <= FLSpace)
			{
				//mustHave <= ValueSum; 인경우에 Case 등록
				if (ValueSpace >= pFlCur->mValue.mMustHaveMin)
				{
					pass = true;
				}
				else
				{
					pass = false;
					goto fail;
				}
			}
			else
			{
				pass = false;
				goto fail;
			}

			if (Value[k]==0) // Value가 없을때
			{
				if (pFlCur->mValue.mMustHaveMin > 0)
				{
					pass = false;
					goto fail;
				}
				else
				{
					pass = true;
				}
			}
			strcat(StrBuffer, ";");
			pFlCur = pFlCur->pNext;
		}
fail:
		if (pass)
		{
			AddToTail(StrBuffer);
		}
	}
	else
	{
		for (int i = 0; i <= mBall; i++)
		{
			if (nThBuket < nBuket)
			{
				Value[nThBuket] = i;
				IntDivideFuc(Value,nBuket, mBall - i, nThBuket + 1);
			}
		}
	}
	return 0;
}

//재귀함수를 단순화
Node<int> *CCaseChecker::CaseGenerartor(Node<CFlagNode> *pFl, Node<int> *pValueHeader, int depth, char *strChar)
{
	char StrBuffer[80];
	strcpy_s(StrBuffer, strChar);
	strcat_s(StrBuffer, ";");	//시작을 표시, 구역에 대한 표시
	//Node<int>	*pStartValue;
	//Node<int>	*pEndValue;
	
	Node<CFlagNode> *CurFl = pFl;
	Node<int> *CurValue = pValueHeader;
	int nFl;
	int nValue;
	for (nFl=0; CurFl; CurFl = CurFl->pNext){nFl++;}	//Fl 개수 구하기 
	for (nValue=0; CurValue; CurValue = CurValue->pNext){nValue++;} // Value 개수구하기 
	
	mpFl = pFl;
	mpValueHeader = pValueHeader;
	if (nValue>0)
	{
		int pValue[30];
		if (pFl)
		{
			IntDivideFuc(pValue, nFl, nValue);
		}		
	}
	return 0;
}

// 재귀호출을 통한 경우의 수를 모두 도출하는 함수

int CCaseNode::SetStatus(int start, int end, int state)
{
	char* buffer = mTable;
	if (start > end)
		return -1;
	for (int i = start; i <=end; i++)
	{
		if(!buffer[i])
			buffer[i] = state;
	}
	return 0;
}


void CCaseChecker::Prepare(char *Table)
{
	Node<CCaseNode> *pCC = mpRootNode;
	while (pCC)
	{
		for (int i = 0; i < MAX_TABLE; i++)
		{
			pCC->mValue.mTable[i] = Table[i];	// 기본 Table설정
		}
		pCC = pCC->pNext;
	}
}

void CCaseChecker::Renderling(CFlagmentation &FL, Node<int> *pValueHeader)
{
	//CCaseChecker *pCaseChecker;
	Node<CCaseNode> *pCaseChecker = mpRootNode;
	Node<CFlagNode> *pFL=NULL;
	Node<int> *pValue;
	Node<int> *pStartValue;
	Node<int> *pEndValue;
	int num = 0;
	int i;
	while (pCaseChecker)
	{
		num++;
		int count = 0;
		pFL = NULL;
		pValue = NULL;
		pStartValue = NULL;
		pEndValue = NULL;			
		for (i = 0; i < strlen(pCaseChecker->mValue.mStrBuffer); i++)
		{
			switch (pCaseChecker->mValue.mStrBuffer[i])
			{
				case '\0':
					goto theNext;
				case ';':
				{
					if (!pFL) // 첫번째에 열고
					{
						pFL = FL.GetHeaderNode();
						pStartValue = NULL;
						pValue = NULL;
					}
					else // 2번째 이후 부터 FL에 대한 처리, ;로 끝나지 않으면 무시됨
					{
						//printf("<FL : [%d,%d]Must %d>\r\n", pFL->mValue.mStart, pFL->mValue.mEnd, pFL->mValue.mMustHaveMin);
						//printf("value count = %d, (%d)\r\n",count,i);
						if (count==0)
						{
							pCaseChecker->mValue.SetStatus(pFL->mValue.mStart, pFL->mValue.mEnd, 2);
						}
						else
						{
							if (pStartValue == NULL)
							{
								goto theNext;
								pStartValue = pValueHeader; 
							}
							//printf("fill : %x ~ %x (%d ea)\r\n", pStartValue->mValue, pEndValue->mValue, count);
							pCaseChecker->mValue.Fill(pFL, pStartValue, pEndValue, 3);
						}

						if (pValue!=NULL)
						{
							pStartValue = pValue->pNext;
						}
						else
						{
							pStartValue = NULL;
						}
						pFL = pFL->pNext;
					}
					count = 0;
				}
				break;
				case ',':
				{
					if (pStartValue == NULL)
					{
						pStartValue = pValueHeader;
						pValue = pStartValue;
						pEndValue = pStartValue;
					}
					else
					{	
						pValue = pValue->pNext;
						pEndValue = pValue;						
					}
					count++;
				}
				break;
				default:	
					;
			}			
		}
	theNext:
	
		pCaseChecker = pCaseChecker->pNext;
	}
	
	pCaseChecker = mpRootNode;
	while (pCaseChecker)
	{
		pCaseChecker = pCaseChecker->pNext;
	}

	
}



int CCaseNode::CaseGenerartor(Node<CFlagNode> *pFl, Node<int> *pStartValue, Node<int> *pEndValue, char *strChar)
{
	
	memcpy(mStrPartialBuffer, strChar,80); // Default 조건들을 받아야함.  FL범위 안의 데이터만 복사해야 함. 
	
	Node<CFlagNode> *CurFl = pFl;
	Node<int> *CurValue = pStartValue;

	int FLSpace = pFl->mValue.mEnd - pFl->mValue.mStart+1;
	int nValue;
	int nBlank;
	int nSumValue=0;
	if (pFl->mValue.mMustHaveMin == 0)	// 값이 없으면 할 필요가 없음. 
	{
	//	return -1; 
	}
	_strnset(mStrOutputBuffer, 0, 80);	
	bStrOutputBufferEmpty = true;

	for (int j = pFl->mValue.mStart; j <= pFl->mValue.mEnd; j++)
	{
		mStrPartialBuffer[j- pFl->mValue.mStart] = mTable[j];	//구간의 Patten을 하나 복사
	}

	for (nValue = 0; ; CurValue = CurValue->pNext)
	{ 
		nValue++; 
		nSumValue += CurValue->mValue;
		if (CurValue == pEndValue)
			break;
	
	} // Value 개수구하기 

	nBlank = FLSpace - nSumValue;
	mpFl = pFl;
	mpValueHeader = pStartValue;
	if (nValue>0)
	{
		int BlankList[80];
		// Blank개수는 Value보다 하나 더 많이
		IntDivideFuc1(BlankList, nValue+1, FLSpace, pStartValue, pEndValue, nBlank,0);
	}
	if (bStrOutputBufferEmpty == false)
	{
		for (int j = pFl->mValue.mStart; j <= pFl->mValue.mEnd; j++)
		{
			mTable[j]= mStrOutputBuffer[j - pFl->mValue.mStart];			
		}
	}
	return 0;
}

// 한 FL 내에서 Value를 어떻게 나눌것인지 Case 따져봄
int CCaseNode::IntDivideFuc1(int *BlankList, int nBuket, int FLSpace,Node<int> *pStartValue, Node<int> *pEndValue,int RemainBlank, int nThBuket = 0)
{
	if (RemainBlank > -1)
	{
		// 정수분할,  n개 버켓에 m개볼.	
		if (nThBuket >= nBuket - 1)
		{
			BlankList[nThBuket] = RemainBlank;
			char StrBuffer[100];
			Node<int> *pValue = pStartValue;
			int k;
			int startAddr = 0;
			for (k = 0; k < nBuket - 1; k++)
			{
				memset(StrBuffer + startAddr, 2, BlankList[k]);	// blank로 설정
				memset(StrBuffer + startAddr + BlankList[k], 3, pValue->mValue);	// blank로 설정
				startAddr += (BlankList[k] + pValue->mValue);
				pValue = pValue->pNext;
			}
			memset(StrBuffer + startAddr, 2, BlankList[k]);
			bool pass = false;
			for (k = 0; k < FLSpace; k++)
			{
				if ((mStrPartialBuffer[k] == 3) || (mStrPartialBuffer[k] == 4))
				{
					if (StrBuffer[k] != 3)
					{
						pass = false;
						break;
					}
					else if (StrBuffer[k] == 3)
					{
						pass = true;
					}
				}
			}

			if (pass)
			{
				if (bStrOutputBufferEmpty)
				{
					memcpy(mStrOutputBuffer, StrBuffer, 80);
					bStrOutputBufferEmpty = false;
				}
				else
				{
					for (int j = 0; j < FLSpace; j++)
					{
						if (StrBuffer[j] == 3 && mStrOutputBuffer[j]==3)
						{
							mStrOutputBuffer[j] = 3;
						}
						else if (StrBuffer[j] ==2 && mStrOutputBuffer[j] == 2)
						{
							mStrOutputBuffer[j] =2;
						}
						else
						{
							mStrOutputBuffer[j] = 0;
						}
					}
				}
			}
		}
		else
		{
			int i;
			i = (nThBuket == 0) ? 0 : 1;
			for (i; i <= FLSpace; i++)
			{
				if (nThBuket < nBuket)
				{
					BlankList[nThBuket] = i;
					//      (int *BlankList, int nBuket, int FLSpace, Node<int> *pStartValue, Node<int> *pEndValue, int RemainBlank, int nThBuket = 0)
					IntDivideFuc1(BlankList, nBuket, FLSpace, pStartValue, pEndValue, RemainBlank - i, nThBuket + 1);
				}
			}	// for i
		} // 
	}
	return 0;
}


int CCaseNode::Fill(Node<CFlagNode>* pFL, Node<int> *pStartValue, Node<int> *pEndValue, int state)
{
#if 1
	if (!pStartValue)
		return -1;
	int ValueCount = pStartValue->mValue;
	int NumOfValue = 1;
	int nCount = pFL->mValue.mEnd - pFL->mValue.mStart + 1; // 구간의 크기
	int ValueMax=0; // 구간 값중 가장 큰값
	int ValueMin = 256;
	int ValueSum = 0;
	Node<int> *pValue = pStartValue;

	//printf("[Fill]\r\n");


	if (pEndValue->pNext == pStartValue) // 불가해보이는데??
	{
		SetStatus(pFL->mValue.mStart, pFL->mValue.mEnd,2);
		return 1;
	}

	for( ;pValue != pEndValue->pNext; pValue = pValue->pNext)
	{
		if (!pValue) // Value가 없는경우 렌더링 할 것도 없다. 
			return -1;
		if (ValueMax < pValue->mValue)
			ValueMax = pValue->mValue;	

		if (ValueMin > pValue->mValue)
			ValueMin = pValue->mValue;
		ValueSum += pValue->mValue;
		NumOfValue++;
	}
	NumOfValue--;
	pValue = pStartValue;
	int first = -1;	// 처음으로 정해진 값이 나오기 시작하는 지점 
	int last = -1;	// 마지막으로 정해진 값이 
	int last_blank = -1;	// 마지막으로 정해진 값이 
	int first_blank = -1;	// 처음으로 정해진 값이 나오기 시작하는 지점 
	

	int first_length = 0;
	int last_length = 0;
	int first_blank_length = 0;
	int last_blank_length = 0;
	int _last_blank_length = 0;
	int _last_length = 0;
	for (int j = pFL->mValue.mStart; j <= pFL->mValue.mEnd; j++)
	{
		
		if (mTable[j] > 2)	//선택된 것이 나오기 시작하는 점 
		{
			if (first < 0)	
				first = j;	// 처음 나오면 first
			_last_length++;
			last_length = _last_length;

			last = j;
			_last_blank_length = 0;
		}

		else 
		{
			if (!mTable[j] && first_blank < 0)
			{
				first_blank = j;
			}
			if (mTable[j]>0)
			{
				last_blank = j;				
				_last_blank_length++;
				last_blank_length = _last_blank_length;
			}
			else
			{
				if ((first_blank > -1) && first_blank_length == 0)
				{
					first_blank_length = last_blank - first_blank + 1;
				}				
				_last_blank_length = 0;
			}
			if ((first > -1) && first_length == 0)
			{
				first_length = last - first + 1;
			}			
			_last_length = 0;			
		}
	}
	if (_last_blank_length > 0 && !last_length)
	{
		last_length = _last_blank_length;
	}



	pValue = pStartValue;
	
#if 1
	if (first > 0)
	{
		if ((first_length == pValue->mValue) && (first - pFL->mValue.mStart < pValue->mValue + 1))
		{
			if (first < first_blank) // 공백없이 시작된 처음
			{
				//printf("첫 Value처리\n");
				SetStatus(first + pValue->mValue, first + pValue->mValue, 2);
			}
			else if (first_blank_length < pValue->mValue + 1)
			{
				SetStatus(first - 1, first - 1, 2);
				SetStatus(first + pValue->mValue, first + pValue->mValue, 2);
			}			
		}
		if((first_length < pValue->mValue) && (first_blank< pValue->mValue ))
		{
			SetStatus(first , pFL->mValue.mStart+pValue->mValue-1, 3);
		}
	}
#endif
	
#if 1
	if (last > 0)
	{
		if ((last_length == pEndValue->mValue) && (pFL->mValue.mEnd - last < pEndValue->mValue+1))
		{
			if (last > last_blank) // 공백보다 뒤에나온거면 끝난 마지막
			{	
				SetStatus(last - pEndValue->mValue, last - pEndValue->mValue, 2);
			}
			else if ((last_blank_length < pEndValue->mValue + 1 ))
			{
				SetStatus(last + 1, last + 1, 2);
				SetStatus(last - pEndValue->mValue, last - pEndValue->mValue, 2);
			}		
		}
		if ((last_length < pEndValue->mValue) && (last_blank< pEndValue->mValue))
		{
			SetStatus(pFL->mValue.mEnd - pEndValue->mValue+1  ,last, 3);
		}
	}
#endif
	


#if 1
	if (pValue->mValue == pFL->mValue.mEnd - pFL->mValue.mStart + 1)
	{
		SetStatus(pFL->mValue.mStart, pFL->mValue.mEnd, 3);	
	}
	if (pFL->mValue.mEnd - pFL->mValue.mStart + 1 < ValueMin)
	{
		SetStatus(pFL->mValue.mStart, pFL->mValue.mEnd , 2);
	}
#endif


	if (NumOfValue>1) // value가 1이상일때 
	{
		if (ValueSum + NumOfValue - 1 == pFL->mValue.mEnd - pFL->mValue.mStart + 1)	// 꽉차는경우
		{
			SetStatus(pFL->mValue.mStart, pFL->mValue.mStart, 3);	// 한점만찍으면 알아서 풀릴듯
			//SetStatus(pFL->mValue.mEnd, pFL->mValue.mEnd, 3);	//
		}

		if (first_length >1)
		{
			if (first_length == ValueMax)
			{
				SetStatus(first-1, first-1, 2);
				SetStatus(first + first_length , first + first_length , 2);
			}
			
		}

		if (last_length >1)
		{
			if (last_length == ValueMax)
			{
				SetStatus(last + 1, last + 1, 2);
				SetStatus(last - last_length, last - last_length, 2);
			}			
				
		}
	}
	else if (NumOfValue==1)
	{
		if (last - first < ValueMax)
		{
			SetStatus(first, last, 3);	
		}

		if (pFL->mValue.mMustHaveMin)
		{
			if ((pFL->mValue.mEnd - pFL->mValue.mStart+1) == ValueCount)
			{
				SetStatus(pFL->mValue.mStart, pFL->mValue.mEnd, 3);
			}
			
			
			if (first < 0 || last < 0)
			{
				SetStatus(pFL->mValue.mEnd - pValue->mValue + 1, pFL->mValue.mStart + pValue->mValue - 1, 3);
			}
			else
			{

				SetStatus( first, pFL->mValue.mStart + pValue->mValue-1, 3);
				SetStatus( pFL->mValue.mEnd - pValue->mValue+1, last, 3);
				SetStatus(pFL->mValue.mEnd - pValue->mValue + 1, first - 1, 3);

				if (last - first  < pValue->mValue)
				{
					SetStatus(first, last, 3);
					SetStatus(first + pValue->mValue, pFL->mValue.mEnd, 2);
					SetStatus(pFL->mValue.mStart, last - pValue->mValue, 2);
				}
				if ((first_length == pValue->mValue) && (first - pFL->mValue.mStart < pValue->mValue + 1))
				{
					SetStatus(pFL->mValue.mStart, first, 2);
					SetStatus(first + pValue->mValue, first + pValue->mValue, 2);
				}
				
			}
		}
		else
		{
			SetStatus(pFL->mValue.mEnd - ValueCount + 1, pFL->mValue.mStart + ValueCount - 1, 3);
		}
	}
	else
	{
		  SetStatus(pFL->mValue.mStart , pFL->mValue.mEnd , 2);
	}

	//Item3 : 가정법 
	int pre_count;
	int next_count;
	next_count = ValueSum;
	pre_count = 0;
	pValue = pStartValue;	
	next_count -= pValue->mValue;
	for (int i = 0; i < NumOfValue; i++)
	{
		if ((nCount - next_count - NumOfValue - i - pValue->mValue+1 ) <= pre_count + pValue->mValue -1 )
		{
			SetStatus(pFL->mValue.mStart+nCount - next_count - NumOfValue - i - pValue->mValue+1 , pFL->mValue.mStart+pre_count + pValue->mValue-1 , 3);	//가로 
		}
		pre_count += pValue->mValue + 1;
		pValue = pValue->pNext;
		if (pValue)
			next_count -= (pValue->mValue + 2);
	}

	// 마지막엔 무식한 방법으로 
#endif
	if(NumOfValue<8)
		CaseGenerartor(pFL, pStartValue, pEndValue, mTable);
	
	return 0;
}

void CCaseNode::table_merge(char* inTable1, char* inTable2, char *outTable,int Count)
{
	int i;

	for (i = 0; i < Count; i++)
	{
		if (inTable1[i] == inTable2[i])
			outTable[i] = inTable2[i];
		else
			outTable[i] = 0;
	}
	return;
}

Node<CCaseNode>* CCaseChecker::AddToTail(char *str)
{
	Node<CCaseNode> *pNewNode = new Node<CCaseNode>;	
	strcpy_s(pNewNode->mValue.mStrBuffer, str);
	CLinkList<CCaseNode>::AddToTail(pNewNode);
	return pNewNode;
}

void CCaseNode::Set(Node<int> *start, Node<int> *end)
{
	pValueStart = start;
	pValueEnd = end;
}
