#[Data Manager]60053804 ▶ DM_BO_LOC_3170_BOOK_INFO_SE
ALIAS=DM_BO_LOC_3170_BOOK_INFO_SE
CONNECTION_INFO=kolas2@k2up/kolas2#[]
TBL_NAME=SE_BOOK_TBL BB , IDX_SE_TBL IB
CONDITION=BB.SPECIES_KEY = IB.REC_KEY
EXTRA_CONDITION=
#Field Alias|Field Name|Field Type|InitValue|Data Type|
IBS_청구_도서기호|BB.BOOK_CODE|str|||
IBS_청구_분류기호|BB.CLASS_NO|str|||
IBS_청구_복본기호|BB.COPY_CODE|str|||
책KEY|BB.REC_KEY|int|||
등록번호|BB.REG_NO|str|||
IBS_청구_별치기호_코드|BB.SEPARATE_SHELF_CODE|str|||
IBS_청구_권책기호|BB.VOL_CODE|str|||
배가상태|BB.WORKING_STATUS|str|||
MARC|BS.MARC|str||UDF|
저작자|IB.AUTHOR_INFO|str|||
발행자|IB.PUB_INFO|str|||
종KEY|IB.REC_KEY|str|||
본표제|IB.TITLE_INFO|str|||
청구기호||||UDF|
