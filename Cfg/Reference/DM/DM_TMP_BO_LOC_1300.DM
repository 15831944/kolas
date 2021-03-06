#[Data Manager]50029508 ▶ DM_TMP_BO_LOC_1300
ALIAS=DM_TMP_BO_LOC_1300
CONNECTION_INFO=kolas2@k2up/kolas2#[]
TBL_NAME=BO_BOOK_TBL BB , IDX_BO_TBL IB , BO_SPECIES_TBL BS
CONDITION=BB.SPECIES_KEY =IB.REC_KEY AND BB.SPECIES_KEY = BS.REC_KEY AND IB.MANAGE_CODE=UDF_MANAGE_CODE
EXTRA_CONDITION=
#Field Alias|Field Name|Field Type|InitValue|Data Type|
IBS_청구_도서기호|BB.BOOK_CODE|str|||
IBS_청구_분류기호|BB.CLASS_NO|str|||
BB_원/복본구분|BB.COPY_CODE|str|||
IBS_청구_복본기호|BB.COPY_CODE|str|||
BB_관리구분|BB.MANAGE_CODE|str|||
가격|BB.PRICE|str|||
BB_발행년|BB.PUBLISH_YEAR|str|||
BB_책KEY|BB.REC_KEY|int|||
BB_등록번호|BB.REG_NO|str|||
BB_대표책값|BB.REPRESENT_BOOK_YN|str|||
IBS_청구_별치기호_코드|BB.SEPARATE_SHELF_CODE|str|||
BB_배가변경일자|BB.SHELF_CHANGE_DATE|str|||
BB_배가변경사유내용|BB.SHELF_CHANGE_ETC_REASON|str|||
BB_배가변경사유|BB.SHELF_CHANGE_REASON_CODE|str|||
BB_최초배가일자|BB.SHELF_DATE|str|||
BB_배가기호|BB.SHELF_LOC_CODE|str|||
BB_자료실명|BB.SHELF_LOC_CODE|str|||
BB_배가변경지시자|BB.SHELF_WORKER|str|||
BB_이용제한구분|BB.USE_LIMIT_CODE|str|||
BB_편/권차|BB.VOL|str|||
IBS_청구_권책기호|BB.VOL_CODE|str|||
BB_편제|BB.VOL_TITLE|str|||
BB_배가상태|BB.WORKING_STATUS|str|||
BS_MARC|BS.MARC|str||UDF|
저작자|IB.AUTHOR_INFO|str|||
제어번호|IB.CONTROL_NO|str|||
형식구분|IB.FORM_CODE|str|||
발행자|IB.PUB_INFO|str|||
BS_종KEY|IB.REC_KEY|int|||
본표제|IB.TITLE_INFO|str|||
청구기호||str||UDF|
선정||||UDF|
형태사항||||UDF|
BB_비치신청자|BB.APPLICANT_NAME|str|||
