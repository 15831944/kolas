#[Data Manager]60044577 ▶ DM_SE_LOC_1320
ALIAS=DM_SE_LOC_1320
CONNECTION_INFO=kolas2@k2up/kolas2#[]
TBL_NAME=BO_BOOK_TBL BB , BO_SPECIES_TBL BS
CONDITION=BB.SPECIES_KEY = BS.REC_KEY AND BB.MANAGE_CODE=UDF_MANAGE_CODE
EXTRA_CONDITION=
#Field Alias|Field Name|Field Type|InitValue|Data Type|
형식구분|BB.FORM_CODE|str|||
관리구분|BB.MANAGE_CODE|str|||
매체구분|BB.MEDIA_CODE|str|||
가격|BB.PRICE|str|||
가격성격|BB.PRICE_CHARACTER|str|||
가격잡정보|BB.PRICE_OTHER_INFO|str|||
발행일자|BB.PUBLISH_DATE|str|||
발행년|BB.PUBLISH_YEAR|str|||
등록구분|BB.REG_CODE|str|||
배가변경일|BB.SHELF_CHANGE_DATE|str|||
배가일|BB.SHELF_DATE|str|||
이용제한구분|BB.USE_LIMIT_CODE|str|||
년도구분|BS.ACQUISIT_YEAR|str|||
MARC|BS.MARC|str|||
자료구분|BS.MAT_CODE|str|||
보조등록번호|BS.SUB_REG_CODE|str|||
이용대상구분|BS.USE_OBJ_CODE|str|||
변경사유|SHELF_CHANGE_REASON|str||UDF|
발행지||str||UDF|
발행자||str||UDF|
