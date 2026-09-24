
#define STR_NULL       0             // 当前是NULL字符(及超出字串)
#define STR_HZLEFT     1             // 当前是汉字左半部分
#define STR_HZRIGHT    2             // 当前是汉字右半部分
#define STR_ASCII      3             // 当前是英文字符

int argw=0;
char edit_buf[1024];      // 编辑一行时的临时缓冲区
BYTE HZ_Left=0;             // 汉字输入时用的临时寄存

void ED_showline(EDITOR *edw, EDLINE *edl, int y);
void ED_showstate(EDITOR *edw);
void ED_drawtext(WINDOWS *wnd);
void ED_redraw(WINDOWS *wnd);
void ED_movecurs(void);
void ED_resize(WINDOWS *wnd);
/*
// 检察一个字串的当前位置处是否汉字
int ED_checkhz(char *buf, int curepos)
{
	int i, l;

	if (strlen(buf)<curepos) return STR_NULL;
	if ((BYTE)buf[curepos]<0xa1) return STR_ASCII;
	l=0;
	for (i=0; i<=curepos; i++){
		 if ((BYTE)buf[i]>0xa0) l=1-l;
	}
	if (l) return STR_HZLEFT;
	return STR_HZRIGHT;
}
*/

void edMouse(int x, int y, WORD s)
{
  x=x;
  y=y;
  s=s;
}

int editor(int retkey, char kbstate)
{
	int extk, l, c, i;
	WINDOWS *wnd;
	EDITOR *edw;
	EDLINE *edl;
	WORD old_posY;
	char *tmp;

  kbstate=kbstate;
	wnd=UC_GetCurrentWindow();
	edw=UC_EditWindowFound(wnd);
	extk=retkey&0xff00;        // 扩展键码

	if (!(retkey&0x00ff)) {                // 扩展键码处理
		 old_posY=edw->cure_posY;   // 记录移动前的Y向位置
		 switch (extk) {
				  case SK_ARROWUP:
						 if (edw->cure_posY) edw->cure_posY--;
						 break;
				  case SK_ARROWDOWN:
						 if (edw->cure_posY+1<edw->totle_lines) edw->cure_posY++;
						 break;
				  case SK_ARROWLEFT:
						 if (edw->cure_posX) {
							 edw->cure_posX--;
							 l=UC_CheckEditPoint(edit_buf, edw->cure_posX);
							 if (l==STR_HZRIGHT) edw->cure_posX--;
						 }
						 break;
				  case SK_ARROWRIGHT:
						 if (edw->cure_posX<1023) {
							 edw->cure_posX++;
							 l=UC_CheckEditPoint(edit_buf, edw->cure_posX);
							 if (l==STR_HZRIGHT) edw->cure_posX++;
						 }
						 break;
				  case SK_PAGEUP:
						 if (edw->cure_posY>wnd->vbar->page)
							  edw->cure_posY-=wnd->vbar->page;
						 else edw->cure_posY=0;
						 break;
				  case SK_PAGEDOWN:
						 edw->cure_posY+=wnd->vbar->page;
						 if (edw->cure_posY>=edw->totle_lines)
							  edw->cure_posY=edw->totle_lines-1;
						 break;
				  case SK_HOME:
						 edw->cure_posX=0;
						 break;
				  case SK_END:
						 edw->cure_posX=strlen(edit_buf); //edw->edl_cure->buf);
						 break;
				  case SK_CTRLHOME:
						 edw->cure_posY=wnd->vbar->cure;
						 break;
				  case SK_CTRLEND:
						 edw->cure_posY=wnd->vbar->cure+edw->win_lines-1;
						 if (edw->cure_posY>=edw->totle_lines)
							  edw->cure_posY=edw->totle_lines-1;
						 break;
				  case SK_CTRLPAGEUP:
						 edw->cure_posY=0;
						 break;
				  case SK_CTRLPAGEDOWN:
						 edw->cure_posY=edw->totle_lines-1;
						 break;
				  case SK_DELETE:
						 l=UC_CheckEditPoint(edit_buf, edw->cure_posX);
						 if (l==STR_NULL) return 1;
						 if (l==STR_HZLEFT) {
							 movmem(edit_buf+edw->cure_posX+2,
									  edit_buf+edw->cure_posX,
									  strlen(edit_buf)-edw->cure_posX-1);
						 }
						 else movmem(edit_buf+edw->cure_posX+1,
										 edit_buf+edw->cure_posX,
										 strlen(edit_buf)-edw->cure_posX);
						 ED_showline(edw, edw->edl_cure,
									  (edw->cure_posY-wnd->vbar->cure)*18);
						 goto nasc;
				  default: return NULL;
		 }

		 if (old_posY!=edw->cure_posY) {    // 如果光标上下移动过
			 if (edw->edl_cure)
				if (strcmp(edit_buf, edw->edl_cure->buf))
				  if ((tmp=realloc(edw->edl_cure->buf, strlen(edit_buf)+1))!=NULL){
					  edw->edl_cure->buf=tmp;
					  strcpy(edw->edl_cure->buf, edit_buf);   // 重新分配修改过的行内容
					  edit_buf[0]=0;
				  }
		 }
		 edl=edw->edl_head;
		 for (i=1; i<=edw->cure_posY; i++){
			  edl=edl->next;
		 }
		 edw->edl_cure=edl;                 // 计算当前光标位置对应的指针

		 if (old_posY!=edw->cure_posY) {              // 如果光标上下移动过
			 if (edw->edl_cure) strcpy(edit_buf, edw->edl_cure->buf);     // 将新行内容拷贝到临时编辑缓冲区中
			 else edit_buf[0]=0;
		 }
	} else {
		  if ((retkey&0x00ff)<0x20) {     // 控制字符
			  switch (retkey) {
						case SK_BACKSPACE:
							  if (edw->cure_posX) {
								  edw->cure_posX--;
								  l=UC_CheckEditPoint(edit_buf, edw->cure_posX);
								  if (l==STR_NULL) goto kok;
								  if (l==STR_HZRIGHT) {
									  movmem(edit_buf+edw->cure_posX+1,
												edit_buf+edw->cure_posX-1,
												strlen(edit_buf)-edw->cure_posX);
									  edw->cure_posX--;
								  }
								  else movmem(edit_buf+edw->cure_posX+1,
												  edit_buf+edw->cure_posX,
												  strlen(edit_buf)-edw->cure_posX);
								 ED_showline(edw, edw->edl_cure,
												(edw->cure_posY-wnd->vbar->cure)*18);
							  }
							  goto nasc;
						case SK_ENTER:
							  edl=malloc(sizeof(EDLINE));
							  edl->buf=malloc(1024);
							  if (strlen(edit_buf)>edw->cure_posX)
									strcpy(edl->buf, edit_buf+edw->cure_posX);
							  else edl->buf[0]=0;
							  tmp=realloc(edl->buf, strlen(edl->buf)+1);
							  edl->buf=tmp;
							  UC_EditLineInsert(edw, edw->edl_cure->next, edl);
							  edw->totle_lines++;
							  if (edw->totle_lines>edw->win_lines)      // 按回车后重新计算垂直滚动条最大值
									wnd->vbar->max=edw->totle_lines-edw->win_lines+1;
							  else wnd->vbar->max=1;
//							  UC_DisplayScrollbar(wnd, wnd->hbar);

							  edit_buf[edw->cure_posX]=0;
							  tmp=realloc(edw->edl_cure->buf, strlen(edit_buf)+1);
							  edw->edl_cure->buf=tmp;
							  strcpy(edw->edl_cure->buf, edit_buf);   // 重新分配修改过的行内容
							  strcpy(edit_buf, edl->buf);
							  edw->edl_cure=edl;
							  edw->cure_posY++;
							  edw->cure_posX=0;
							  l=1;
							  goto kok;
			  }
			  return NULL;
		  }

		  while (strlen(edit_buf)<edw->cure_posX) {      // 如果光标处于行长之外
			  strcat(edit_buf, " ");                      // 后续位置需要添加空格补上
		  }
		  if (strlen(edit_buf)>1022) {       // 缓冲区满, 鸣叫退出
			  sound(30); delay(30); nosound(); return 1;
		  }
		  movmem(edit_buf+edw->cure_posX, edit_buf+edw->cure_posX+1,
					strlen(edit_buf)-edw->cure_posX+1);
		  if ((retkey&0xff) > 0xa0) {
			  if (HZ_Left==0) {
					HZ_Left=retkey&0xff;
					edit_buf[edw->cure_posX]=0x20;   //输入汉字左半部份时, 先用空格填充当前位置
			  }
			  else {
					edit_buf[edw->cure_posX-1]=HZ_Left;  //否则可以整个汉字填入
					edit_buf[edw->cure_posX]=retkey&0xff;
					HZ_Left=0;
			  }
		  }
		  else {
					edit_buf[edw->cure_posX]=retkey&0xff;
					HZ_Left=0;
		  }
		  edw->cure_posX++;
		  ED_showline(edw, edw->edl_cure, (edw->cure_posY-wnd->vbar->cure)*18);
	}
nasc:
	l=0;
kok:
	c=edw->cure_posX-wnd->hbar->cure;              // 水平位置检察
	if (c<0) {
		 wnd->hbar->cure=edw->cure_posX;
		 UC_UpdateScrollbar(wnd, wnd->hbar);
		 l=1;
	}
	else if (c>edw->win_coles-1) {
		 wnd->hbar->cure+=c-edw->win_coles+1;
		 UC_UpdateScrollbar(wnd, wnd->hbar);
		 l=1;
	}

	c=edw->cure_posY-wnd->vbar->cure;              // 垂直位置检察
	if (c<0) {
		 wnd->vbar->cure=edw->cure_posY;
		 UC_UpdateScrollbar(wnd, wnd->vbar);
		 edl=edw->edl_head;
		 for (i=1; i<=wnd->vbar->cure; i++) {
			 if (!edl->next) break;
			 edl=edl->next;
		 }
		 edw->edl_top=edl;
		 l=1;
	}
	else if (c>edw->win_lines-1) {
		 wnd->vbar->cure+=c-edw->win_lines+1;
		 UC_DisplayScrollbar(wnd, wnd->vbar);
		 edl=edw->edl_head;
		 for (i=1; i<=wnd->vbar->cure; i++) {
			 if (!edl->next) break;
			 edl=edl->next;
		 }
		 edw->edl_top=edl;
		 l=1;
	}

	UC_MouseHide();
	if (l) ED_drawtext(wnd); //ED_redraw(wnd);
	ED_movecurs();
	UC_MouseShow();
	return 1;
}

// 将文本光标移动到当前文本行列位置,
// 如果光标移动后的位置是超出窗口的, 则自动销毁光标
void ED_movecurs(void)
{
  int l, c, i;
  WINDOWS *wnd;
  EDITOR *edw;

  wnd=UC_GetCurrentWindow();
  edw=UC_EditWindowFound(wnd);
  l=UC_CheckEditPoint(edit_buf, edw->cure_posX);
  if (l==STR_HZRIGHT) edw->cure_posX--;      // 光标对准汉字的左半部分
  l=edw->cure_posY-wnd->vbar->cure;
  c=edw->cure_posX-wnd->hbar->cure;

  if (l<0 || c<0 || l>edw->win_lines || c>edw->win_coles) {
	  UC_DestroyCaret(wnd);   // 超出窗口范围的光标被销毁
	  return;
  }
  if (strlen(edit_buf)<=edw->cure_posX)
		 UC_CreateCaret(wnd, 2, 16);        // 空字符部分的光标
  else if ((BYTE)edit_buf[edw->cure_posX]>0xa0)
		 UC_CreateCaret(wnd, 16, 16);       // 汉字光标
  else UC_CreateCaret(wnd, 8, 16);        // 英文光标
  UC_MoveCaret(wnd, c*8, l*18);
  ED_showstate(edw);
}

void ED_movlr(SCROLLBAR *sbar, float pcure)
{
	WINDOWS *wnd;

	wnd=UC_GetCurrentWindow();
	if (sbar->cure==pcure) return;      //没有变化, 返回
	ED_drawtext(wnd); //ED_redraw(wnd);
	ED_movecurs();
}

void ED_movud(SCROLLBAR *sbar, float pcure)
{
	EDLINE *edl;
	EDITOR *edw;
	WINDOWS *wnd;
	WORD i;

	wnd=UC_GetCurrentWindow();
	if (sbar->cure==pcure) return;      //没有变化, 返回

	edw=UC_EditWindowFound(wnd);
	edl=edw->edl_head;
	for (i=1; i<=sbar->cure; i++) {
		if (!edl->next) break;
		edl=edl->next;
	}
	edw->edl_top=edl;

	ED_drawtext(wnd); //ED_redraw(wnd);
	ED_movecurs();
}

// 显示每个编辑窗口的提示行
void ED_showstate(EDITOR *edw)
{
	RECT rect;
	WINDOWS *wnd;

	wnd=edw->wnd;
	if (wnd->hide) return;
//	UC_MouseHide();
	if (UC_BeginPaintStateLine(wnd)) {
	  UC_GetStateLineRect(wnd, &rect);
//	  setfillstyle(SOLID_FILL, LIGHTGRAY);
//	  UC_WindowBar(wnd, rect.left, rect.top+1, rect.right, rect.bottom);
	  setcolor(0);
	  setcolorbm(7);
	  settextstyle(0, 0, wnd->syschar_size);
	  UC_WindowPrintf(wnd, 10, rect.top+2, DT_OVER, "总行数: %u  当前行: %u  当前列: %u      ",
							(WORD)edw->totle_lines, (WORD)edw->cure_posY+1, (WORD)edw->cure_posX+1);
	  UC_EndPaintStateLine(wnd);
	}
//	UC_MouseShow();
}

// 显示一行文本
// 如果显示光标所在的行时, 则显示临时编辑缓冲区edit_buf中的内容
// y= 开始显示的Y坐标
void ED_showline(EDITOR *edw, EDLINE *edl, int y)
{
	int j, x, i;
	WINDOWS *wnd;
	char *tmp, tmp1[1024];

	settextstyle(0, 0, 16);
	wnd=edw->wnd;
	j=wnd->hbar->cure;

	UC_MouseHide();
	UC_HideCaret(wnd);
	setfillstyle(SOLID_FILL, WHITE);
	setcolor(0);

	if (wnd==UC_GetCurrentWindow() && (edl==edw->edl_cure)) tmp=edit_buf;
	else if (edl) tmp=edl->buf;
	else tmp="";
	if (j<strlen(tmp)) {
		strcpy(tmp1, tmp+j);
		if (UC_CheckEditPoint(tmp, j)==STR_HZRIGHT)
			tmp1[0]=0x20;    // 截掉了汉字的后半部分用空格替代(以免影响后续字符的正常显示)
		if (strlen(tmp1)>edw->win_coles+2) tmp1[edw->win_coles+2]=0; // 截掉超出窗口的字符
		setcolor(0);
		setcolorbm(15);
		UC_WindowPrintf(wnd, 0, y, DT_OVER, tmp1);
	}
	x=-1;
	if (j>strlen(tmp)) x=0;
		else if (textwidth(tmp+j)<wnd->width-wnd->vx-wnd->vr)
					x=textwidth(tmp+j);
	if (x!=-1) UC_WindowBar(wnd, x, y, wnd->width-wnd->vx-wnd->vr-x, 18);
	UC_ShowCaret(wnd);
	UC_MouseShow();
}

void ED_drawtext(WINDOWS *wnd)
{
	struct viewporttype vi;
	EDITOR *edw;
	EDLINE *edl;
	int i, y, x, lh;
	RECT rect;

	settextstyle(0, 0, 16);
	getviewsettings(&vi);
	edw=UC_EditWindowFound(wnd);
	y=0;
	edl=edw->edl_top;
	lh=textheight(edl->buf)+2;
	UC_MouseHide();
	UC_HideCaret(wnd);
	for (i=1; i<=edw->win_lines+1; i++) {
		 if (!(y+lh+wnd->top+wnd->vy<vi.top || y+wnd->top+wnd->vy>vi.bottom)){
			 ED_showline(edw, edl, y);
		 }
		 y+=lh;
		 if (edl) edl=edl->next;
	}
	ED_showstate(edw);
	UC_ShowCaret(wnd);
	UC_MouseShow();
}

void ED_redraw(WINDOWS *wnd)
{
	struct viewporttype vi;
	EDITOR *edw;
	EDLINE *edl;
	int i, y, x, lh;
	RECT rect;

	settextstyle(0, 0, 16);
	getviewsettings(&vi);
	edw=UC_EditWindowFound(wnd);
	y=0;
	edl=edw->edl_top;
	lh=textheight(edl->buf)+2;
	UC_MouseHide();
	UC_HideCaret(wnd);
	for (i=1; i<=edw->win_lines+1; i++){
		 if (!(y+lh+wnd->top+wnd->vy<vi.top || y+wnd->top+wnd->vy>vi.bottom)){
			 ED_showline(edw, edl, y);
		 }
		 y+=lh;
		 if (edl) edl=edl->next;
	}

	if (UC_BeginPaintStateLine(wnd)) {
	  UC_GetStateLineRect(wnd, &rect);
	  setfillstyle(SOLID_FILL, LIGHTGRAY);
	  UC_WindowBar(wnd, rect.left, rect.top+1, rect.right, rect.bottom);
	  UC_EndPaintStateLine(wnd);
	}

	ED_showstate(edw);
	UC_ShowCaret(wnd);
	UC_MouseShow();
}

void ED_close(void)
{
	EDITOR *edw;
	EDLINE *edl;

	edw=UC_EditWindowFound(UC_GetCurrentWindow());
	while (edw->edl_head) {
		  edl=edw->edl_head->next;
		  free(edw->edl_head->buf);
		  free(edw->edl_head);
		  edw->edl_head=edl;
	}
	UC_EditWindowDelete(edw);
	free(edw);
	UC_WindowClose();
}

void calcsize(WINDOWS *wnd)
{
	RECT rect;
	EDITOR *edw;
	EDLINE *edl;
	int i;

	UC_GetClientRect(wnd, &rect);
	edw=UC_EditWindowFound(wnd);
	edw->win_lines=(rect.bottom+1)/18;
	edw->win_coles=(rect.right+1)/8;
	wnd->hbar->max=1024-edw->win_coles;
	if (edw->totle_lines>edw->win_lines)
		 wnd->vbar->max=edw->totle_lines-edw->win_lines+1;
	else wnd->vbar->max=1;
	wnd->vbar->page=edw->win_lines-1;         // 上下翻页尺寸为窗口行数-1
	wnd->hbar->page=edw->win_coles/2;         // 左右翻页尺寸为窗口列数的一半
	if (wnd->vbar->page<1) wnd->vbar->page=1;
	if (wnd->hbar->page<1) wnd->hbar->page=1;

	if (wnd->vbar->cure+edw->win_lines>=edw->totle_lines) {
		if (edw->totle_lines>edw->win_lines+1)
			 wnd->vbar->cure=edw->totle_lines-edw->win_lines; //-1;
		else wnd->vbar->cure=0;
		edl=edw->edl_head;
		for (i=1; i<=wnd->vbar->cure; i++) {
			 if (!edl->next) break;
			 edl=edl->next;
		}
		edw->edl_top=edl;
	}
}

void ED_resize(WINDOWS *wnd)
{
	calcsize(wnd);
	ED_movecurs();
}

int ED_enable(WINDOWS *wnd, int msg)
{
	EDITOR *edw;
	char *tmp;

	edw=UC_EditWindowFound(wnd);
	switch (msg) {
		 case WM_DISABLE:             // 当窗口被禁止时
				if (edw->edl_cure)
				  if (strcmp(edit_buf, edw->edl_cure->buf))
					 if ((tmp=realloc(edw->edl_cure->buf, strlen(edit_buf)+1))!=NULL){
						 edw->edl_cure->buf=tmp;
						 strcpy(edw->edl_cure->buf, edit_buf);   // 重新分配修改过的行内容
						 edit_buf[0]=0;
					 }
				break;
		 case WM_ENABLE:              // 当窗口被激活时
				if (edw->edl_cure) strcpy(edit_buf, edw->edl_cure->buf);     // 将新行内容拷贝到临时编辑缓冲区中
				else edit_buf[0]=0;
				break;
	}
	return NULL;
}

//void funok()
//{
//	UC_WindowClose();
//}

WINDOWS *ED_defwin(void)
{
	WINDOWS *wnd;
	EDITOR *edw;

	if ((edw=malloc(sizeof(EDITOR)))==NULL) {
		UC_DialogWarning(NULL, NULL, "内存不够!");
		return NULL;
	}
	if ((edw->edl_cure=malloc(sizeof(EDLINE)))==NULL) {
		free(edw);
		UC_DialogWarning(NULL, NULL, "内存不够!");
		return NULL;
	}
	if ((edw->edl_cure->buf=malloc(1024))==NULL) {
		free(edw->edl_cure);
		free(edw);
		UC_DialogWarning(NULL, NULL, "内存不够!");
		return NULL;
	}

	wnd=UC_DefineWindow(WS_MAIN,
							  CP_USEDEFAULT, CP_USEDEFAULT,
							  CW_USEDEFAULT, CW_USEDEFAULT,
							  "无标题",
							  ED_redraw,
							  ED_close,
							  ED_resize);
	UC_DefineWindowEnable(wnd, ED_enable);    // 定义窗口转换时的处理函数
	UC_DefineWindowIcon(wnd, mulpad, 3, NULL);
	UC_DefineWindowHScrollbar(wnd, ED_movlr);
	UC_DefineWindowVScrollbar(wnd, ED_movud);
	UC_DefineStateLine(wnd, 20);
	UC_DefineKeyboardEvent(wnd, editor);   // 定义编辑器键盘事件函数
  UC_DefineUserMouseEvent(wnd, 0, 0, 0, 0, USM_LEFT,
                          UC_GetIDC(IDC_IBEAM), edMouse);

	edw->wnd=wnd;
	edw->changed=0;
	edw->edl_head=NULL;
	edw->edl_tail=NULL;
	edw->cure_posX=0;
	edw->cure_posY=0;
	edw->totle_lines=1;    // 初始总行数应该等于1
	UC_EditWindowAppend(edw);
	UC_EditLineAppend(edw, edw->edl_cure);
	ED_resize(wnd);    // 根据窗口尺寸计算窗口行列数
	edit_buf[0]=0;     // 新文件, 初始化临时编辑缓冲区为空
	edw->edl_cure->buf[0]=0;
	edw->edl_top=edw->edl_head;
	return wnd;
}

void ED_NewFile(void)
{
	WINDOWS *wnd;

	if ((wnd=ED_defwin())==NULL) return;
	UC_WindowEnable(wnd);
	ED_movecurs();
}

void openfail()
{
}

void openok(char *newfilename)
{
	EDITOR *edw;
	EDLINE *edl;
	WINDOWS *wnd;
	int handle, i, bytes, j;

	if ((handle=open(newfilename, O_BINARY | O_RDONLY))==-1) return;

	if ((wnd=ED_defwin())==NULL) {
		close(handle);
		return;
	}
	edw=UC_EditWindowFound(UC_GetCurrentWindow());
	strcpy(edw->filepath, newfilename);

	wnd->title=edw->filepath;
	wnd->ico_text=edw->filepath;

	if (!filelength(handle)) {    // 文件长度0字节
		close(handle);             // 当做新文件打开
		UC_WindowEnable(wnd);
		return;
	}

	edw->totle_lines=0;
	UC_EditLineDelete(edw, edw->edl_cure);       // 删除预定义行
	free(edw->edl_cure->buf);
	free(edw->edl_cure);
	edw->edl_top=edw->edl_head=edw->edl_tail=edw->edl_cure=NULL;

	UC_MouseShapeType(IDC_WAIT);
	while(1) {
		 bytes=read(handle, edit_buf, 1022);
		 if (bytes==0 || bytes==-1) break;
		 for (i=0; i<bytes; i++) {
			if (edit_buf[i]==0x0d || edit_buf[i]==0x0a || edit_buf[i]==0x1a) {
				if (edit_buf[i]!=0x1a) edit_buf[i]=0;      // 取消第一个换行
					i++;
				break;
			}
		 }
		 edit_buf[i]=0;    // 取消第二个回车
		 lseek(handle, -(bytes-i-1), SEEK_CUR);

		 bytes=strlen(edit_buf);
		 for (i=0; i<bytes; i++) {
			if (edit_buf[i]==0x09) {   // 制表符(TAB)位对齐
//				edit_buf[i]=0x20;
				j=i;
				while (j>=8) j=j-8;
				j=8-j;
				if (j>1) {
					 j--;
					 memmove(edit_buf+i+j+1, edit_buf+i+1, bytes-i);
					 memset(edit_buf+i, 0x20, j+1);
					 bytes+=j+1;
				}
				else {
					 bytes+=j;
				}
			}
		 }

		 if ((edl=malloc(sizeof(EDLINE)))==NULL) break;
		 if ((edl->buf=malloc(strlen(edit_buf)+1))==NULL) {
			 free(edl);
			 break;
		 }
		 strcpy(edl->buf, edit_buf);
		 UC_EditLineAppend(edw, edl);
		 edw->totle_lines++;
	}
	UC_MouseShapeType(IDC_ARROW);
	close(handle);
	edw->edl_top=edw->edl_head;
	edw->edl_cure=edw->edl_head;
	strcpy(edit_buf, edw->edl_head->buf);      // 初始化临时编辑缓冲区为当前文件第一行

//	ED_resize(wnd);    // 根据窗口尺寸计算窗口行列数
	calcsize(wnd);
	UC_WindowEnable(wnd);
	ED_movecurs();
}

void ED_OpenFile(void)
{
  static char *arg[]={"*.txt", "*.doc", "*.*", NULL};

  UC_DialogFileOpen(LOADFILE, &argw, arg, NULL, openok, openfail);
}

void ED_Save(void)
{
}

void ED_SaveAs(void)
{
}

void ED_SaveAll(void)
{
}

void ED_Print(void)
{
}

void ED_DosShell(void)
{
	UC_ToDosPrompt();
}

void ED_Quit(void)
{
	UC_CloseUCVision(NULL, NULL);
}

