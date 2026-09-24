
// 每一行文本的结构
typedef struct _edl{
	char *buf;               // 本行缓冲区指针
	struct _edl *prev;       // 前一行结构指针
	struct _edl *next;       // 后一行结构
}EDLINE;

// 每个窗口对应的编辑文本结构
typedef struct _edw{
	WINDOWS *wnd;                  // 对应的窗口句柄
	char filepath[MAXPATH];        // 编辑的文件名及其路径
	char changed;                  // 是否被修改过标记(1=是)
	EDLINE *edl_head, *edl_tail;   // 头尾指针
	EDLINE *edl_top;               // 窗口顶部指针
	WORD cure_posX;                // 当前光标所在的文本列数
	WORD cure_posY;                // 当前光标所在的文本行数
	EDLINE *edl_cure;              // 光标所在的当前行指针
	WORD totle_lines;              // 总共的行数
	WORD win_lines;                // 窗口可显示的行数
	WORD win_coles;                // 窗口可显示的列数
	struct _edw *prev;
	struct _edw *next;
}EDITOR;

EDITOR *edw_head, *edw_tail;

// 追加一个编辑窗结构
void UC_EditWindowAppend(EDITOR *edw)
{
	edw->prev=edw->next=NULL;
	if (edw_tail) {
		 edw->prev=edw_tail;
		 edw_tail->next=edw;
	}
	edw_tail=edw;
	if (!edw_head) edw_head=edw;
}

// 删除一个编辑窗结构
void UC_EditWindowDelete(EDITOR *edw)
{
	if (edw->next) edw->next->prev = edw->prev;
	if (edw->prev) edw->prev->next = edw->next;
	if (edw_head==edw) edw_head = edw->next;
	if (edw_tail==edw) edw_tail = edw->prev;
	edw->next = edw->prev = NULL;
}

// 将一新行结构追加到链表中
// edl=新行结构指针
void UC_EditLineAppend(EDITOR *edw, EDLINE *edl)
{
	edl->prev=edl->next=NULL;
	if (edw->edl_tail) {
		 edl->prev=edw->edl_tail;
		 edw->edl_tail->next=edl;
	}
	edw->edl_tail=edl;
	if (!edw->edl_head) edw->edl_head=edl;
}

// 将一行结构链中删除
// edl= 欲删除的行指针
void UC_EditLineDelete(EDITOR *edw, EDLINE *edl)
{
	if (edl->next) edl->next->prev = edl->prev;
	if (edl->prev) edl->prev->next = edl->next;
	if (edw->edl_head==edl) edw->edl_head = edl->next;
	if (edw->edl_tail==edl) edw->edl_tail = edl->prev;
	edl->next = edl->prev = NULL;
}

// 在当前行前插入一个新行结构(插入并不会影响尾指针)
// cedl= 当前行
// nedl= 新的行
void UC_EditLineInsert(EDITOR *edw, EDLINE *cedl, EDLINE *nedl)
{
	if (cedl==NULL) {
		nedl->prev=edw->edl_tail;
		nedl->next=NULL;
		edw->edl_tail->next=nedl;
		edw->edl_tail=nedl;
		return;
	}
	nedl->prev=cedl->prev;
	if (cedl->prev) cedl->prev->next=nedl;
	nedl->next=cedl;
	cedl->prev=nedl;
  if (!nedl->prev) edw->edl_head=nedl;
}

// 找出与当前窗口对应的编辑窗口结构
EDITOR *UC_EditWindowFound(WINDOWS *wnd)
{
  EDITOR *edw;

  if (!wnd) return NULL;
  edw=edw_tail;
  while (edw) {
		  if (wnd==edw->wnd) return edw;
		  edw=edw->prev;
  }
  return NULL;
}
