#[Data Manager]73491780 ▶ DM_USER_DUPLECATE_COPY
ALIAS=DM_USER_DUPLECATE_COPY
CONNECTION_INFO=
TBL_NAME=CO_LOAN_USER_TBL CLU
CONDITION=
EXTRA_CONDITION=
#Field Alias|Field Name|Field Type|InitValue|Data Type|
REC_KEY|REC_KEY|str|||
대출자번호|USER_NO|str|||
이름|NAME|str|||
생년월일|TO_CHAR(BIRTHDAY,'YYYY/MM/DD') A|str|||
휴대폰|HANDPHONE|str|||
이메일|E_MAIL|str|||
집전화|H_PHONE|str|||
등록일자|TO_CHAR(REG_DATE,'YYYY/MM/DD HH24:MI:SS') A|str|||
아이디|USER_ID|str|||
성별|DECODE(GPIN_SEX,'0','남','1','여','') A|str|||
실명인증코드|IPIN_HASH|str|||
예약중|(SELECT COUNT(1) FROM LS_WORK_T01 WHERE USER_KEY=CLU.REC_KEY AND STATUS='3') A|str|||
대출중|(SELECT COUNT(1) FROM LS_WORK_T01 WHERE USER_KEY=CLU.REC_KEY AND STATUS IN ('0','2','L','D')) A|str|||
중복사유||str||udf|
회원상태|DECODE(USER_CLASS,'0','정상회원','1','대출정지','2','제적회원','3','탈퇴회원','알수없음') A|str|||
ID중복||str||udf|
실명인증중복||str||udf|
이름생일중복||str||udf|
이름휴대폰중복||str||udf|