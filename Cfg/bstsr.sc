<html>
<head>

<style>
/* �⺻ ��Ʈ */
body, td, form, input, select, option {font-family:����; font-size:12px; color:#6a6a6a; line-height:18px}
.txts						{font-family:����; font-size:11px; text-decoration:none; line-height:14px; color:#777777;}
.txts-blk					{font-family:����; font-size:11px; text-decoration:none; line-height:14px; color:#404040;}
.txts-orange			{font-family:����; font-size:11px; text-decoration:none; line-height:14px; color:#F66B00;}

/* �⺻ �ѿ��� */
a:active		{font-size:12px; color:#6a6a6a; text-decoration:none; line-height:14px;}
a:link			{font-size:12px; color:#6a6a6a; text-decoration:none; line-height:14px;}
a:visited		{font-size:12px; color:#6a6a6a; text-decoration:none; line-height:14px;}
a:hover			{font-size:12px; color:#347fd2; text-decoration:underline; line-height:14px;}

/* ���� ���� ��Ÿ�� #1 */
.BookTitle1							{text-decoration:none; line-height:14px;}
.BookTitle1 a:active		{color:#2d2d2d; font-weight:bold; text-decoration:none; line-height:14px;}
.BookTitle1 a:link			{color:#2d2d2d; font-weight:bold; text-decoration:none; line-height:14px;}
.BookTitle1 a:visited		{color:#2d2d2d; font-weight:bold; text-decoration:none; line-height:14px;}	
.BookTitle1 a:hover			{color:#51b74a; font-weight:bold; text-decoration:underline; line-height:14px;}

/* ���� ���� ��Ÿ�� #2 */
.BookTitle2							{text-decoration:none; line-height:14px;}
.BookTitle2 a:active		{color:#2d2d2d; text-decoration:none; line-height:14px;}
.BookTitle2 a:link			{color:#2d2d2d; text-decoration:none; line-height:14px;}
.BookTitle2 a:visited		{color:#2d2d2d; text-decoration:none; line-height:14px;}	
.BookTitle2 a:hover			{color:#51b74a; text-decoration:underline; line-height:14px;}

/* ���� , ������, ������, ���ǻ� */
.Price				 	{color:#ff620d; font-weight:bold; line-height:14px;}
.PriceThin		 	{color:#ff620d; font-weight:normal; line-height:14px;}
.Discount				{color:509AE5; font-weight:normal; line-height:14px;}
.Bonus					{color:#69a3d3; font-weight:bold; line-height:14px;}
.Publisher			{color:#ababab; font-weight:normal; line-height:14px;}

/* ���� ���� ���� */
.Notice								{color:#6e93bd; text-decoration:none; line-height:14px;}
.Notice a:active			{color:#6e93bd; text-decoration:none; line-height:14px;}
.Notice a:link				{color:#6e93bd; text-decoration:none; line-height:14px;}
.Notice a:visited			{color:#6e93bd; text-decoration:none; line-height:14px;}	
.Notice a:hover				{color:#385776; text-decoration:none; line-height:14px;}

/* ���� ������ å�ҽ� */
.BookNews							{color:#9e9180; text-decoration:none; line-height:14px;}
.BookNews a:active		{color:#9e9180; text-decoration:none; line-height:14px;}
.BookNews a:link			{color:#9e9180; text-decoration:none; line-height:14px;}
.BookNews a:visited		{color:#9e9180; text-decoration:none; line-height:14px;}	
.BookNews a:hover			{color:#706353; text-decoration:none; line-height:14px;}

/* �������� ���� �̺�Ʈ���� */
.BookEventWorld						{font-size:11px; color:#4a4a4a; letter-spacing:-1px; line-height:14px;}
.BookEventWorld	a:active	{font-size:11px; color:#4a4a4a; letter-spacing:-1px; line-height:14px;}
.BookEventWorld	a:link		{font-size:11px; color:#4a4a4a; letter-spacing:-1px; line-height:14px;}
.BookEventWorld	a:visited	{font-size:11px; color:#4a4a4a; letter-spacing:-1px; line-height:14px;}
.BookEventWorld	a:hover		{font-size:11px; color:#347fd2; letter-spacing:-1px; line-height:14px;}

/* ������������ Ư������ ���� ��Ÿ�� */
.BookTitle_SP							{color:#ffffff; font-weight:bold; text-decoration:none; line-height:14px;}
.BookTitle_SP a:active		{color:#ffffff; font-weight:bold; text-decoration:none; line-height:14px;}
.BookTitle_SP a:link			{color:#ffffff; font-weight:bold; text-decoration:none; line-height:14px;}
.BookTitle_SP a:visited		{color:#ffffff; font-weight:bold; text-decoration:none; line-height:14px;}	
.BookTitle_SP a:hover			{color:#51b74a; font-weight:bold; text-decoration:underline; line-height:14px;}

/* �˻���� ī�װ� �ڽ� ��Ÿ�� */
.SearchCate							{color:#7e7e7e; text-decoration:none; line-height:14px;}
.SearchCate a:active		{color:#7e7e7e; text-decoration:none; line-height:14px;}
.SearchCate a:link			{color:#7e7e7e; text-decoration:none; line-height:14px;}
.SearchCate a:visited		{color:#7e7e7e; text-decoration:none; line-height:14px;}	
.SearchCate a:hover			{color:#347fd2; text-decoration:underline; line-height:14px;}

/* �˻���� ī�װ� ��ħ���̾�*/
.SearchCateClass				{display:none;}

/* ������ �׺���̼� */
.PageNavi							{color:#a8a8a8; font-weight:bold; text-decoration:none; line-height:14px; letter-spacing:-1px;}
.PageNavi a:active		{color:#a8a8a8; font-weight:Normal; text-decoration:none; line-height:14px; letter-spacing:-1px;}
.PageNavi a:link			{color:#a8a8a8; font-weight:Normal; text-decoration:none; line-height:14px; letter-spacing:-1px;}
.PageNavi a:visited		{color:#a8a8a8; font-weight:Normal; text-decoration:none; line-height:14px; letter-spacing:-1px;}	
.PageNavi a:hover			{color:#797979; font-weight:bold; text-decoration:none; line-height:14px; letter-spacing:-1px;}

/* �˻���� ���� ���*/
.Publisher2			{color:#2d2d2d; font-weight:normal; line-height:14px;}

/* �з��� ����Ʈ ���̾� �ѿ��� 1 */
.GenreList1							{font-size:11px; color:#7e9f1e; text-decoration:none; line-height:14px;}
.GenreList1 a:active		{font-size:11px; color:#7e9f1e; text-decoration:none; line-height:14px;}
.GenreList1 a:link			{font-size:11px; color:#7e9f1e; text-decoration:none; line-height:14px;}
.GenreList1 a:visited		{font-size:11px; color:#7e9f1e; text-decoration:none; line-height:14px;}	
.GenreList1 a:hover			{font-size:11px; color:#305b14; text-decoration:none; line-height:14px;}

/* �з��� ����Ʈ ���̾� �ѿ��� 2 */
.GenreList2							{font-size:11px; color:#9b9b9b; text-decoration:none; line-height:14px;}
.GenreList2 a:active		{font-size:11px; color:#9b9b9b; text-decoration:none; line-height:14px;}
.GenreList2 a:link			{font-size:11px; color:#9b9b9b; text-decoration:none; line-height:14px;}
.GenreList2 a:visited		{font-size:11px; color:#9b9b9b; text-decoration:none; line-height:14px;}	
.GenreList2 a:hover			{font-size:11px; color:#000000; text-decoration:none; line-height:14px;}

/* �α��� ��â ���ڽ� */
.LoginForm		{border:1px #bdbdbd solid;}

/* ȸ������ ���ڽ� */
.JoinForm		{border:1px #bdbdbd solid;}

/* �ֹ����� ���ڽ� */
.PayForm		{border:1px #dedede solid;}

/* �˻� ���ڽ� */
.SearchForm		{border:1px #dedede solid;}

/* �����̽� DB Box �η����� */
.ShowcaseGift			{color:#ababab; line-height:12px;}

/* �з� ����Ʈ ���� �ؽ�Ʈ */
.ReviewTextMain			{color:#999999;}
.ReviewTextAuthor		{color:#696969;}
.ReviewTextvBar			{color:#d9d9d9;}

/* ��ǰ �� �ٷΰ��� �ѿ��� */
.Detail_Link							{font-size:12px; color:#968d83; text-decoration:none; line-height:14px;}
.Detail_Link a:active			{font-size:12px; color:#968d83; text-decoration:none; line-height:14px;}
.Detail_Link a:link				{font-size:12px; color:#968d83; text-decoration:none; line-height:14px;}
.Detail_Link a:visited		{font-size:12px; color:#968d83; text-decoration:none; line-height:14px;}	
.Detail_Link a:hover			{font-size:12px; color:#777068; text-decoration:underline; line-height:14px;}

/* �˻���� ���ù�ư �ѿ��� */
.Search_Sort							{font-size:12px; color:#94c8fd; text-decoration:none; line-height:14px;}
.Search_Sort a:active			{font-size:12px; color:#94c8fd; text-decoration:none; line-height:14px;}
.Search_Sort a:link				{font-size:12px; color:#94c8fd; text-decoration:none; line-height:14px;}
.Search_Sort a:visited		{font-size:12px; color:#94c8fd; text-decoration:none; line-height:14px;}	
.Search_Sort a:hover			{font-size:12px; color:#ffffff; text-decoration:none; line-height:14px;}

/* �˸� �޼��� */
.AlertText				{color:#ff7f31;}

/* ��ٱ��ϴ�� ���̾� �ѿ��� */
.InCartOn					{font-size:12px; color:#585858; background-color:#f5f5f5; text-decoration:none; line-height:14px; cursor:hand;}
.InCartOff				{font-size:12px; color:#949494; background-color:#ffffff; text-decoration:none; line-height:14px; cursor:hand;}

/* ��ٱ��ϴ�� ���̾� �ѿ��� ������ */
.InCartThinOn					{font-size:11px; letter-spacing:-2px; color:#585858; background-color:#f5f5f5; text-decoration:none; line-height:14px; cursor:hand;}
.InCartThinOff				{font-size:11px; letter-spacing:-2px; color:#949494; background-color:#ffffff; text-decoration:none; line-height:14px; cursor:hand;}

/* ���̷� ���� ��� ��ǰ �ѿ��� */
.Myroom_HotGoods							{font-size:11px; color:#c7c7c7; text-decoration:none; line-height:14px;}
.Myroom_HotGoods a:active			{font-size:11px; color:#c7c7c7; text-decoration:none; line-height:14px;}
.Myroom_HotGoods a:link				{font-size:11px; color:#c7c7c7; text-decoration:none; line-height:14px;}
.Myroom_HotGoods a:visited		{font-size:11px; color:#c7c7c7; text-decoration:none; line-height:14px;}	
.Myroom_HotGoods a:hover			{font-size:11px; color:#ffee62; text-decoration:none; line-height:14px;}

/* ȸ�����Ծ�� */
.member                 {font-family:����; font-size:12px; color:#6a6a6a; line-height:18px}


.reviewText P {MARGIN-TOP: 2px; MARGIN-BOTTOM: 2px;}

P {MARGIN-TOP: 2px; MARGIN-BOTTOM: 2px;}

</style>


</head>

<body leftmargin="0" topmargin="0" marginwidth="0" marginheight="0">

			<table width="100%" align="center" cellspacing="0" cellpadding="0" border="0">
				<tr><td><img src="http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/tit_todayBest.gif" border="0"></td></tr>
				<tr><td style="padding:3 0 10 14; border-left:#DDDDDD solid 1px;border-right:#DDDDDD solid 1px;">

<SCRIPT language=javascript>
//Math.random()
var scrollerheight=119;  // ��ũ�ѷ��� ���� 
var html,total_area=0,wait_flag=true; 
var bMouseOver = 1;
var scrollspeed = 1;  // Scrolling �ӵ�         
var waitingtime = 9000;  // ���ߴ� �ð�
var s_tmp = 0, s_amount = 119;
var scroll_content=new Array();
var startPanel=0, n_panel=0, i=0;


function startscroll()
{ // ��ũ�� ����
  i=0;
  for (i in scroll_content)
   n_panel++;

   
  n_panel = n_panel -1 ;
  startPanel = Math.round(Math.random()*n_panel);
  if(startPanel == 0)
  {
   i=0;
   for (i in scroll_content) 
    insert_area(total_area, total_area++); // area ����
  }
  else if(startPanel == n_panel)
  {
   insert_area(startPanel, total_area);
   total_area++;
   for (i=0; i<startPanel; i++) 
   {
    insert_area(i, total_area); // area ����
    total_area++;
   }
  }
  else if((startPanel > 0) || (startPanel < n_panel))
  {
   insert_area(startPanel, total_area);
   total_area++;
   for (i=startPanel+1; i<=n_panel; i++) 
   {
    insert_area(i, total_area); // area ����
    total_area++;
   }
   for (i=0; i<startPanel; i++) 
   {
    insert_area(i, total_area); // area ����
    total_area++;
   }
  }
  window.setTimeout("scrolling()",waitingtime);
}
function scrolling(){ // ������ ��ũ�� �ϴ� �κ�
  if (bMouseOver && wait_flag)
  {
   for (i=0;i<total_area;i++){
    tmp = document.getElementById('scroll_area'+i).style;
    tmp.top = parseInt(tmp.top)-scrollspeed;
    if (parseInt(tmp.top) <= -scrollerheight){
     tmp.top = scrollerheight*(total_area-1);
    }
    if (s_tmp++ > (s_amount-1)*scroll_content.length){
     wait_flag=false;
     window.setTimeout("wait_flag=true;s_tmp=0;",waitingtime);
    }
   }
  }
  window.setTimeout("scrolling()",1);
}

function insert_area(idx, n){ // area ����
  html='<div style="left: 0px; width: 100%; position: absolute; top: '+(scrollerheight*n)+'px" id="scroll_area'+n+'">';
  html+=scroll_content[idx];
  html+='</div>';
  document.write(html);
}

function getStringByByte(str, limit_length) {
    if (str == "") {
        return  str;
    }
    var len = 0;
    for (var i = 0; i < str.length; i++) {
        if (str.charCodeAt(i) > 128) {
            len++;
        }
        len++;
        if(len >= limit_length) {
        	return (str.substring(0,(i+1))+"..");
        } 
    }
    return  str;
}
</SCRIPT>

<SCRIPT language=javascript>
��
var scroll_content = new Array();
scroll_content[0]="<img src='"+my_array[0].img+"' align=absmiddle><a href='"+my_array[0].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[0].txt,15)+"</a><br>";
scroll_content[0]+="<img src='"+my_array[1].img+"' align=absmiddle><a href='"+my_array[1].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[1].txt,15)+"</a><br>";
scroll_content[0]+="<img src='"+my_array[2].img+"' align=absmiddle><a href='"+my_array[2].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[2].txt,15)+"</a><br>";
scroll_content[0]+="<img src='"+my_array[3].img+"' align=absmiddle><a href='"+my_array[3].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[3].txt,15)+"</a><br>";
scroll_content[0]+="<img src='"+my_array[4].img+"' align=absmiddle><a href='"+my_array[4].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[4].txt,12)+"</a><br>";
scroll_content[0]+="<img src='"+my_array[5].img+"' align=absmiddle><a href='"+my_array[5].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[5].txt,15)+"</a><br>";
scroll_content[1]="<img src='"+my_array[6].img+"' align=absmiddle><a href='"+my_array[6].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[6].txt,15)+"</a><br>";
scroll_content[1]+="<img src='"+my_array[7].img+"' align=absmiddle><a href='"+my_array[7].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[7].txt,15)+"</a><br>";
scroll_content[1]+="<img src='"+my_array[8].img+"' align=absmiddle><a href='"+my_array[8].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[8].txt,15)+"</a><br>";
scroll_content[1]+="<img src='"+my_array[9].img+"' align=absmiddle><a href='"+my_array[9].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[9].txt,15)+"</a><br>";
scroll_content[1]+="<img src='"+my_array[10].img+"' align=absmiddle><a href='"+my_array[10].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[10].txt,15)+"</a><br>";
scroll_content[1]+="<img src='"+my_array[11].img+"' align=absmiddle><a href='"+my_array[11].link+"'><img src=http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/blank.gif border=0>"+getStringByByte(my_array[11].txt,15)+"</a><br>";

</SCRIPT>
<DIV id=scroll_image onmouseover=bMouseOver=0 style="OVERFLOW: hidden; WIDTH: 147px; HEIGHT: 119px" onmouseout=bMouseOver=1>
<div style="position:relative; WIDTH: 147px; HEIGHT: 119px"> <SCRIPT>startscroll();</SCRIPT></div>
</DIV> 
</td></tr>
				 <tr><td><img src="http://image.kyobobook.co.kr/newimages/apps/b2c/welcome/new/dot_line01.gif" border="0"></td></tr>

			</table>
			</div>

</body>
</html>