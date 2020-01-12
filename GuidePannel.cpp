#include "stdafx.h"
#include "GuidePannel.h"
#include "CaseChecker.h"
//#define MAX_TABLE 100

#define NEXT_ITEM(pTile) ((!direction) ? pTile->pRight : pTile->pDown)
#define PREV_ITEM(pTile) ((!direction) ? pTile->pLeft : pTile->pUp)
CGuidePannel::CGuidePannel():mRemainValues(0),mNumOfValue(0)
{
	strcpy(name, "GudPannel");
	pNext = NULL;
	pHeader = NULL;
}

CGuidePannel::~CGuidePannel()
{
	if (pHeader)
		delete pHeader;
	
	if (pNext)
		delete pNext;
	pNext = NULL;
	
}

CBoardTile *CGuidePannel::MoveToNext(int direction, CBoardTile *pCurTile)
{
	if (direction)
		if (pCurTile->pDown)	//ver
			pCurTile = pCurTile->pDown;		
	else
		if (pCurTile->pRight)	//Hor
			pCurTile = pCurTile->pRight;		
	return pCurTile;
}

CBoardTile *CGuidePannel::MoveToNextWithNoneProtect(int direction, CBoardTile *pCurTile)
{
	if (direction)
			pCurTile = pCurTile->pDown;
	else
			pCurTile = pCurTile->pRight;
	return pCurTile;
}


CBoardTile *CGuidePannel::MoveToPrev(int direction, CBoardTile *pCurTile)
{
	if (direction == 0)	// Hor
	{
		if (pCurTile->pLeft)
			pCurTile = pCurTile->pLeft;
	}
	else
	{
		if (pCurTile->pUp)
			pCurTile = pCurTile->pUp;
	}
	return pCurTile;
}


void CGuidePannel::AddValue(int value)
{
	
	CGuideValue *pTail;
	if (!pHeader)
	{
		CGuideValue *pValue = new CGuideValue;
		pValue->mValue = value;
		pHeader = pValue;
	}
	else
	{
		CGuideValue *pValue = new CGuideValue;
		pValue->mValue = value;
		pTail = pHeader->GetTail();
		pTail->pNext = pValue;
		pValue->pPrev = pTail;
	}
}

CGuideValue  *CGuidePannel::GetValue(int index)
{
	CGuideValue *pValue;
	if (index < 0)
		return NULL;
	if (!pHeader)
	{
		return NULL;
	}
	pValue = pHeader;
	for (int i = 0; i < index; i++)
	{
		if (!pValue->pNext)
			return NULL;
		pValue = pValue->pNext;
		
	}
	return pValue;
}

int CGuidePannel::PannelStatusCheck(int nCount, CBoardTile *pTile,int direction)
{
	int i;
	
	int *Normal_fixed_sequence = new int[nCount];
	int *Backside_fixed_sequence = new int[nCount];
	int *Total_Sequence = new int[nCount];
	BOOL bOn;

	CBoardTile *pCurTile;
	CBoardTile *pTailTile;
	pCurTile = pTile;
	int itemCount;

	//initialize
	for (i = 0; i < nCount; i++)
	{
		Normal_fixed_sequence[i] = 0;
		Backside_fixed_sequence[i] = 0;
		Total_Sequence[i]=0;
	}

	//Full Searching
	bOn = FALSE;
	for (i = 0, itemCount = 0; i < nCount; i++)
	{
		if (pCurTile->GetStatus() == 3)
		{
			bOn = TRUE;
			Total_Sequence[itemCount]++;
		}
		else if (bOn)
		{
			bOn = FALSE;
			itemCount++;
		}		
		pCurTile = MoveToNext(direction, pCurTile);
		
	}
	pTailTile = pCurTile;	// 끝 타일


	//처음부터 확정타일만 검색
	pCurTile = pTile;
	bOn = FALSE;
	for (i = 0, itemCount = 0; i < nCount; i++)
	{
		if (pCurTile->GetStatus()>0)	//뭔가라도있으면 진행
		{			
			if (pCurTile->GetStatus() >= 3)
			{				
				bOn = TRUE;
				Normal_fixed_sequence[itemCount]++;
			}
			else if(bOn)
			{
				bOn = FALSE;
				itemCount++;
			}
		}
		else 
		{
			bOn = FALSE;
			break;
		}
		pCurTile = MoveToNext(direction, pCurTile);		
	}

	//끝부터 확정타일만 검색
#if 1
	pCurTile = pTailTile;
	bOn = FALSE;
	for (i = 0, itemCount = 0; i < nCount; i++)
	{
		if (pCurTile->GetStatus()>0)	//뭔가라도있으면 진행
		{
			if (pCurTile->GetStatus() >= 3)
			{
				bOn = TRUE;
				Backside_fixed_sequence[itemCount]++;
			}
			else if (bOn)
			{
				bOn = FALSE;
				itemCount++;
			}
		}
		else
		{
			bOn = FALSE;
			break;
		}

		pCurTile = MoveToPrev(direction, pCurTile);		
	}
#endif
	// 가이드와 비교
	CGuideValue *pValue = pHeader;
	
	for (i = 0; i < mNumOfValue; i++)
	{
		pValue->mAchieve = 0;
		pValue = pValue->pNext;
	}
	pValue = pHeader;

	int count = 0;
	int sum_of_value = 0;
	int sum_of_value_1 = 0;
	for (i = 0; i < mNumOfValue; i)
	{
		if (pValue->mValue >0)
		{
		
			sum_of_value += pValue->mValue;
			sum_of_value_1 += Total_Sequence[i];
		
			if (pValue->mValue == Normal_fixed_sequence[i])
			{
				if (Normal_fixed_sequence[i] > 0)
				{
					//count++;
					pValue->mAchieve = 1;
				}
			}
		}
		i++;
		pValue = pValue->pNext;
		if (!pValue)
			break;
	}

#if 1
	pValue = pHeader;
	
	if (sum_of_value == sum_of_value_1)
	{		
		for (i = 0; i < mNumOfValue; i++)
		{
			if (pValue->mValue == Total_Sequence[i])
			{
				pValue->mAchieve = 1;
			}
			else
			{
				break;
			}
			pValue = pValue->pNext;
			if (!pValue)
			break;
		}	
		//intf("\r\n");
	}
#endif	
#if 1
	pValue = pHeader->GetTail();
	for (i = 0; i < mNumOfValue; i)
	{
		if (pValue->mValue == Backside_fixed_sequence[i])
		{
			pValue->mAchieve = 1;
			i++;
			if (!Backside_fixed_sequence[i])
				break;
		}
		else
		{
			break;
		}
		pValue = pValue->pPrev;
		if (!pValue)
			break;
	}
#endif
	pValue = pHeader;
	count = 0;
	for (i = 0; i < mNumOfValue; i++)
	{
		if (pValue->mAchieve) count++;
		pValue = pValue->pNext;
	}

	delete[] Normal_fixed_sequence;
	delete[] Backside_fixed_sequence;
	delete[] Total_Sequence;
	mRemainValues = mNumOfValue - count;
	return mNumOfValue - count;
}

int CGuidePannel::SetState(CBoardTile *pTile, int Start, int End, int StateValue,int direction)
{
	CBoardTile *pCurTile = pTile;
	int index = 0;
	int status;
	while (pCurTile)
	{
		if ((Start <= index) && (index<=End))
		{
			status = pCurTile->GetStatus();
			if (status == 0)
				pCurTile->SetStatus(StateValue);
		}	
		pCurTile = MoveToNextWithNoneProtect(direction, pCurTile);		
		index++;
	}
	return 0;
}
int  CGuidePannel::Draw_FL_info(CDC *pDC, CBoardTile *pTile,int direction, Node<CFlagNode> *pFL)
{
	int i = 0;
	CRect rectStart; 
	CRect rectEnd;
	CString strtext;
	for (pTile; pTile;i++)
	{
		if (i == pFL->mValue.mStart)
		{
			rectStart = pTile->GetPosition();
		}
		if(i == pFL->mValue.mEnd)
		{
			rectEnd = pTile->GetPosition();
		}
		pTile =MoveToNextWithNoneProtect(direction, pTile);
	}
	pDC->MoveTo(rectStart.CenterPoint().x+2, rectStart.top+2);
	pDC->LineTo(rectEnd.CenterPoint().x-2, rectEnd.top+2);

	return 0;
}

int CGuidePannel::PannelAutoStatusCheck(int nCount, CBoardTile *pTile, int direction)
{
	CGuideValue *pValue = pHeader;
	CBoardTile *pCurTile;	
	char debug_buffer[80];

	int i,j;
	int	count = 0;
	int total_count = 0;
	int total;
	int empty_count = 0;
	int first_position = -1;
	int last_position = -1;
	int set_count = 0;
	int left_count=0;
	int right_count=0;
	pCurTile = pTile;


	while (NEXT_ITEM(pTile))
	{
		if ((pTile->GetStatus() == 1) || (pTile->GetStatus() == 2))
		{
			left_count++;
		}
		else
		{			
			pTile = pTile->MoveToTileEnd(direction);			
			break;
		}
		pTile = NEXT_ITEM(pTile);
	}

	while (PREV_ITEM(pTile))
	{
		if ((pTile->GetStatus() == 1) || (pTile->GetStatus() == 2))
		{
			right_count++;
		}
		else
		{
			pTile = pTile->MoveToTileFront(direction);
			break;
		}
		pTile = PREV_ITEM(pTile);
	}

	pValue = pHeader;
	for (i = 0; i < mNumOfValue; i++)
	{
		if (pValue->mAchieve)
		{
			count++;		
		}
		total_count += pValue->mValue;
		pValue = pValue->pNext;
	}
	total = total_count;
	total_count += (mNumOfValue - 1);
	pCurTile = pTile;
	i = 0;
	while (pCurTile)
	{
		debug_buffer[i] = pCurTile->GetStatus();
		if (pCurTile->GetStatus() == 0)
		{
			empty_count++;
		}
		if (pCurTile->GetStatus() > 2)
		{
			if (first_position < 0)
				first_position = i;
			last_position = i;
			set_count++;
		}
		i++;
		pCurTile = NEXT_ITEM(pCurTile);//// pCurTile->pRight;
	}

	//Item1 : 완성 행/열 나머지 체크
#if 1
	if (mNumOfValue == count)
	{
		pCurTile = pTile;
		for (i = 0; i < nCount; i++)
		{
			if (pCurTile->GetStatus() == 0)
			{
				pCurTile->SetStatus(2);
			}
			pCurTile = NEXT_ITEM(pCurTile);
		}
	}
#endif

#if 1
	//Item2 : 빈틈없이 이미 결정난경우
	pValue = pHeader;
	if (total_count == nCount)
	{
		pCurTile = pTile;
		for (i = 0; i < mNumOfValue; i++)
		{
			for (j = 0; j < pValue->mValue; j++)
			{
				pCurTile->SetStatus(3);				
				pCurTile = NEXT_ITEM(pCurTile);
			}			
			if (pCurTile)
			{
				pCurTile->SetStatus(2);				
				pCurTile = NEXT_ITEM(pCurTile);
			}
			if (pValue)		pValue = pValue->pNext;
		}
	}
#endif
	//Item3 : 하나의 페널일때 빈공간의 크기가 단일값과 같을경우
#if 1
	pValue = pHeader;
	if (mNumOfValue==1)
	{
		if (empty_count == total_count - set_count)
		{
			pCurTile = pTile;
			for (i = 0; i < mNumOfValue; i++)
			{				
				for (j = 0; j < pValue->mValue; j++)
				{
					if (!pCurTile)
						break;
					if (pCurTile->GetStatus() == 0)
					{
						pCurTile->SetStatus(3);
					}
					else
					{
						j--;						
						pCurTile = NEXT_ITEM(pCurTile);
						continue;
					}					
					pCurTile = NEXT_ITEM(pCurTile);
				}
				if (pCurTile)
				{
					pCurTile->SetStatus(2);
					pCurTile = NEXT_ITEM(pCurTile);

				}
				if (pValue)		pValue = pValue->pNext;
			}

		}

	}
#endif


#if 1
	//Item3 : 가정법 
	int pre_count;
	int next_count;
	next_count = total;
	pre_count = 0;
	pValue = pHeader;
	pCurTile = pTile; 
	next_count -= pValue->mValue;
	for (i = 0; i < mNumOfValue; i++)
	{ 
		if ((nCount -right_count - next_count - mNumOfValue  - i - pValue->mValue+1) <= pre_count + pValue->mValue-1+left_count)
		{
			SetState(pTile, ( nCount - right_count - next_count - mNumOfValue  - i - pValue->mValue+1), pre_count + pValue->mValue-1+left_count, 3, direction);	//가로 
		}
		pre_count += pValue->mValue + 1;
		pValue = pValue->pNext;
		if (pValue)
			next_count -= (pValue->mValue+2);		
	}
#endif

	pValue = pHeader;
	if (first_position > -1)
	{
		if (first_position- left_count  < pValue->mValue)
		{
			SetState(pTile, first_position, pValue->mValue - 1, 3, direction);
			if(first_position==0)
				SetState(pTile, pValue->mValue , pValue->mValue , 2, direction);
		}
	}
	pValue = pHeader->GetTail();
	if (last_position > -1)
	{
		if (last_position > nCount - pValue->mValue)
		{
			SetState(pTile, nCount - pValue->mValue, last_position, 3, direction);
			if (last_position == nCount-1)
				SetState(pTile, nCount - pValue->mValue-1, nCount - pValue->mValue-1, 2, direction);
		}
	}

	if (mNumOfValue == 1)
	{
		if (first_position > -1 && last_position > -1)
		{
			SetState(pTile, first_position, last_position, 3, direction);
		}
	}

	//TRACE("\r\n");\
	//확정된 블럭다음 체크로 닫아주기 (정방향)


	return 0;
}



void CGuidePannel::AI(int nCount, CBoardTile *pTile, int direction)
{
	CValuesList ValueList;
	CGuideValue *pGValue = pHeader;
	CCaseChecker CaseChecker;
	CFlagmentation FL;
	Node<CCaseNode>  *pCurCase;

	char StrBuffer[256];
	bool bOpen = false;
	char buffer[MAX_TABLE];
	char MustHave_buffer[MAX_TABLE];
	char output_buffer[MAX_TABLE];


	CBoardTile *pCurTile;

	pCurTile = pTile;
	int count = 0;
	int sum_of_value = 0;
	for (int i = 0; i < nCount; i++)
	{
		buffer[i] = pCurTile->GetStatus();		
		pCurTile = MoveToNextWithNoneProtect(direction, pCurTile);
	}
	
	Node<CFlagNode> *pFL;
	CCase *pCase = NULL;

	// Test bench
	memset(StrBuffer, 0, 256);

	int start;
	int end;
	int i;
	int flagIndex = 0;
	
	int mustHave = 0;
	// 사전 준비 Flagmentation을 작성	
	for (int i = 0; i < mNumOfValue; i++)
	{
		Node<int> *pValue = new Node<int>;
		pValue->mValue = pGValue->mValue;
		ValueList.AddToTail(pValue);
		pGValue = pGValue->pNext;
	}

	for (i = 0; i < nCount; i++)
	{
		if (buffer[i] == 1 || buffer[i] == 2) // 체크면 Flagment를 닫을수 있음
		{
			if (bOpen) // close Flagmentation
			{
				end = i - 1;
				pFL = new Node<CFlagNode>;
				pFL->mValue.Set(start, end, mustHave);
				FL.AddToTail(pFL);
				flagIndex++;
			}
			mustHave = 0;
			bOpen = false;
		}
		else // Empthy 이거나 확정이면 Flagement 를 시작할 수 있음 
		{
			if (buffer[i] == 3 || buffer[i] == 4)
			{
				mustHave++;
			}
			if (!bOpen)
			{
				bOpen = true;
				start = i;
			}
		}
	}
	if (bOpen) // 열린 상태에서 마친경우 close, 
	{
		end = i - 1;
		Node<CFlagNode> *pFL = new Node<CFlagNode>;
		pFL->mValue.Set(start, end, mustHave);
		FL.AddToTail(pFL);
		flagIndex++;
	}


	//Case generator
	CaseChecker.CaseGenerartor(FL.GetHeaderNode(), ValueList.GetHeaderNode(), 0, StrBuffer);
	CaseChecker.Prepare(buffer);
	CaseChecker.Renderling(FL, ValueList.GetHeaderNode());
	int Nth = 0;
	for (pCurCase = CaseChecker.GetHeaderNode(); 
		pCurCase;Nth++, pCurCase = pCurCase->pNext)
	{
		int count_null_check = 0;
		for (i = 0; i < nCount; i++)
		{
			count_null_check += pCurCase->mValue.mTable[i];
		}
		if (count_null_check)
		{
			pCurCase->mValue.table_merge(output_buffer, pCurCase->mValue.mTable, output_buffer, nCount);
		}
	} //pCurCase

	
	pCurCase = CaseChecker.GetHeaderNode();
	
	if(pCurCase)
	{
		for (int i = 0; i < nCount; i++)
		{
			output_buffer[i] = pCurCase->mValue.mTable[i];
		}
		int caseNum = 0;
		int Nth = 0;
		for (pCurCase; pCurCase;Nth++)
		{
			int count_null_check=0;
			for (i = 0; i < nCount; i++)
			{
				count_null_check += pCurCase->mValue.mTable[i];
			}
			if(count_null_check)
			{
				// Display debug info
				//DrawDebugInfor(pCurCase->mValue.mTable, Nth,direction);
				pCurCase->mValue.table_merge(output_buffer, pCurCase->mValue.mTable, output_buffer, nCount);
			}
			else
			{
			//	printf("NULL data");
			}
			
			caseNum++;
			pCurCase = pCurCase->pNext;
		} // if pCurCase
		


		pCurTile = pTile;
		for (int i = 0; i < nCount; i++)
		{
			CString state;
			pCurTile->Update(output_buffer[i]);
			pCurTile = MoveToNextWithNoneProtect(direction, pCurTile);			
		}		


	}	//pCurCase
	else
	{	//! pCurCase

		//for (int i = 0; i < nCount; i++)
		//{
		//	output_buffer[i] = cc->mValue.mTable[i];
		//}
	}
	//Get reult from each Case

	//Get And Result
	FL.DeleteAllNode();
	//delete(pFLHeader);
	CaseChecker.DeleteAllNode();

	ValueList.DeleteAllNode();	
	//Printf

}



void CGuidePannel::GetCurrentFL(int nCount, int currentPosition, CBoardTile *pTile, int direction, Node<CFlagNode> *pCurrentFL)
{
	bool bOpen = false;
	char buffer[MAX_TABLE];

	CBoardTile *pCurTile;
	pCurTile = pTile;
	for (int i = 0; i < nCount; i++)
	{
		buffer[i] = pCurTile->GetStatus();
		pCurTile = MoveToNextWithNoneProtect(direction, pCurTile);
	}
	CCase *pCase = NULL;
	int start;
	int end;
	int i;
	int fragIndex = 0;
	int mustHave = 0;


	for (i = 0; i < nCount; i++)
	{
		if (buffer[i] == 1 || buffer[i] == 2) // 체크면 Flagment를 닫을수 있음
		{
			if (bOpen) // close Flagmentation
			{
				end = i - 1;
				if (start <= currentPosition && currentPosition <= end)
				{
					pCurrentFL->mValue.Set(start, end, mustHave);
					pCurrentFL->mValue.mIndex = fragIndex;
				}		
				fragIndex++;
			}
			mustHave = 0;
			bOpen = false;
		}
		else // Empthy 이거나 확정이면 Flagement 를 시작할 수 있음 
		{
			if (buffer[i] == 3 || buffer[i] == 4)
			{
				mustHave++;
			}
			if (!bOpen)
			{
				bOpen = true;
				start = i;
			}
		}
	}
	if (bOpen) // 열린 상태에서 마친경우 close, 
	{
		end = i - 1;
		if (start <= currentPosition && currentPosition <= end)
		{
			pCurrentFL->mValue.Set(start, end, mustHave);
			pCurrentFL->mValue.mIndex = fragIndex;
		}
		fragIndex++;
	}		
}
