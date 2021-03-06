#[Data Manager]72995196 ▶ DM_SHELF_LOC_REG_SE
ALIAS=DM_SHELF_LOC_REG_SE
CONNECTION_INFO=kolas2@k2up/kolas2#[]
TBL_NAME=SE_BOOK_TBL BB , IDX_SE_TBL IB , SE_VOL_TBL V , SE_SPECIES_TBL S
CONDITION=BB.SPECIES_KEY = S.REC_KEY AND V.REC_KEY = BB.VOL_KEY AND IB.REC_KEY = BB.SPECIES_KEY AND BB.BOOK_TYPE IN ('B','C','A')
EXTRA_CONDITION=ORDER BY IB.TITLE_INFO
#Field Alias|Field Name|Field Type|InitValue|Data Type|
IBS_청구_별치기호_코드|BB.SEPARATE_SHELF_CODE|str|||
IBS_청구_분류기호|BB.CLASS_NO|str|||
IBS_청구_도서기호|BB.BOOK_CODE|str|||
IBS_청구_권책기호|BB.VOL_CODE|str|||
IBS_청구_복본기호|BB.COPY_CODE|str|||
청구기호||||UDF|
서명|IB.TITLE_INFO|str|||
저자|IB.AUTHOR_INFO|str|||
발행자|IB.PUB_INFO|str|||
발행년|IB.PUB_YEAR|str|||
서가번호|BB.SHELF_NUM|str|||
행번호|BB.SHELF_ROW|str|||
등록번호|BB.REG_NO|str|||
권호명|V.IDX_VOL_TITLE|str|||
