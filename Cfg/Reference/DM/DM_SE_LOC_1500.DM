#[Data Manager]60039137 ▶ DM_SE_LOC_1500
ALIAS=DM_SE_LOC_1500
CONNECTION_INFO=kolas2@k2up/kolas2#[]
TBL_NAME=SE_BOOK_TBL B , SE_SPECIES_TBL S , IDX_SE_TBL I , SE_VOL_TBL V
CONDITION=B.VOL_KEY=V.REC_KEY AND B.SPECIES_KEY = S.REC_KEY AND B.SPECIES_KEY=I.REC_KEY AND B.BOOK_TYPE IN ('A','B','C') AND B.TRANSFER_YN = 'Y' AND I.MANAGE_CODE=UDF_MANAGE_CODE
EXTRA_CONDITION=ORDER BY B.SPECIES_KEY,B.VOL_KEY
#Field Alias|Field Name|Field Type|InitValue|Data Type|
관리구분|B.MANAGE_CODE|str|||
이전자료상태|B.PREV_WORKING_STATUS|str|||
책정보KEY|B.REC_KEY|int|||
가등록번호|B.REG_NO|str|||
대표책값|B.REPRESENT_BOOK_YN|str|N|UDF|
IBS_청구_별치기호_코드|B.SEPARATE_SHELF_CODE|str|||
배가변경사유설명|B.SHELF_CHANGE_ETC_REASON|str|||
배가변경사유코드|B.SHELF_CHANGE_REASON_CODE|str|||
최초배가일자|B.SHELF_DATE|str|||
실시간인계일자|B.IMM_LOC_TRANSFER_DATE|str|||
배가변경일자|B.SHELF_CHANGE_DATE|str|||
자료실|B.SHELF_LOC_CODE|str|||
배가담당자|B.SHELF_WORKER|str|||
이용제한구분|B.USE_LIMIT_CODE|str|||
IBS_청구_권책기호|B.VOL_CODE|str|||
자료상태|B.WORKING_STATUS|str|||
간행빈도|I.PUB_FREQ|str|||
발행자|I.PUB_INFO|str|||
종정보KEY|I.REC_KEY|int|||
서명|I.TITLE_INFO|str|||
복귀경로|S.BINDING_YN|str|||
함번호|S.BOX_NAME|str|||
부차적권호명|V.OTHER_VOL_TITLE|str|||
권호KEY|V.REC_KEY|int|||
권호명|V.VOL_TITLE|str|||
청구기호||str||UDF|
선정||||UDF|
레이블출력구분||str|2|UDF|
등록번호|B.REG_NO|str|||
별치기호_코드|B.SEPARATE_SHELF_CODE|str|||
분류기호|B.CLASS_NO|str|||
도서기호|B.BOOK_CODE|str|||
권책기호|B.VOL_CODE|str|||
복본기호|B.COPY_CODE|str|||
책유형|B.BOOK_TYPE|str|||
수입구분별권KEY|B.ACQ_TYPE_VOL_KEY|str|||
부록명|(SELECT A.APPENDIX_NAME FROM SE_APPENDIX_BOOK_TBL A WHERE B.REC_KEY=A.BOOK_KEY)|str|||
부록여부||str||UDF|
수입구분|B.ACQ_CODE|str|||
IBS_청구_도서기호|B.BOOK_CODE|str|||
IBS_청구_분류기호|B.CLASS_NO|str|||
IBS_청구_복본기호|B.COPY_CODE|str|||
