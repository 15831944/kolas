#[Data Manager]73025943 ▶ DM_ILL_RETURN_WE
ALIAS=DM_ILL_RETURN_WE
CONNECTION_INFO=kolas2@k2up/kolas2#[]
TBL_NAME=ILL_TRANS_TBL A, LH_STORAGE_V01 B
CONDITION=A.LOAN_KEY = B.REC_KEY AND A.STATE = '0014' AND A.LOAN_KEY IS NOT NULL AND B.STATUS IN ('L', 'D')
EXTRA_CONDITION=
#Field Alias|Field Name|Field Type|InitValue|Data Type|
트랜잭션번호|A.TRANS_NO|str|||
등록번호|BB.REG_NO|str|||
서명|IB.TITLE_INFO|str|||
청구기호|<$청구기호$>|str|||
제공도서관_이름|(SELECT C.LIB_NAME FROM MN_LIBINFO2_TBL C WHERE C.LIB_CODE = A.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_LIBNAME|str|||
복귀예정일|TO_CHAR(A.RETURN_PLAN_DATE, 'YYYY/MM/DD')|str|||
신청자의견|A.APL_USER_CONTENT|str|||
요청도서관_이름|(SELECT C.LIB_NAME FROM MN_LIBINFO2_TBL C WHERE C.LIB_CODE = A.APL_LIBCODE AND ROWNUM = 1) AS APL_LIBNAME|str|||
대출일|TO_CHAR(B.LOAN_DATE, 'YYYY/MM/DD')|str|||
반납예정일|TO_CHAR(B.RETURN_PLAN_DATE, 'YYYY/MM/DD')|str|||
대출반납키|A.LOAN_KEY|str|||
제공도서관_코드|A.GIVE_LIBCODE|str|||
대출상태|B.STATUS|str|||
제공도서관_관리구분|(SELECT C.MANAGE_CODE FROM MN_LIBINFO2_TBL C WHERE C.LIB_CODE = A.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGECODE|str|||
부록여부|DECODE(A.BOOK_APPENDIX_FLAG, 'A', 'Y', 'B', 'N')|str|||
모체자료_키|A.SPECIES_KEY|str|||
