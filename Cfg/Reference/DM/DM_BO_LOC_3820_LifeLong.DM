#[Data Manager]73318863 ▶ DM_BO_LOC_3820_LifeLong
ALIAS=DM_BO_LOC_3820_LifeLong
CONNECTION_INFO=kolas2@k2up/kolas2#[]
TBL_NAME=EMT_MEMBER@EMT M, PG_MEMBER@EMT PM, PG_PROGRAM@EMT P, PG_CATEGORY@EMT C, CO_LOAN_USER_TBL CLU
CONDITION=CLU.USER_ID = M.MB_ID AND M.MB_ID = PM.MB_ID AND P.ABSNO = PM.PG_CODE AND P.CATEGORY = C.CODE AND M.MB_MAILLING = 1
EXTRA_CONDITION=
#Field Alias|Field Name|Field Type|InitValue|Data Type|
회원명|M.MB_NAME|str|||
회원ID|M.MB_ID|str|||
성별|DECODE (M.MB_SEX , 'M', '남자', 'F', '여자')|str|||
주소|M.MB_ADDR1|str|||
상세주소|M.MB_ADDR2|str|||
E-MAIL|M.MB_EMAIL|str|||
전화번호|M.MB_TEL|str|||
핸드폰|M.MB_HP|str|||
강좌명|P.PG_NAME|str|||
강좌구분|C.NAME|str|||
선정||str||UDF|
회원주소||str||UDF|
프로그램키|P.ABSNO|int|||
