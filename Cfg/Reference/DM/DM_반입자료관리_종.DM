#[Data Manager]60050167 ▶ DM_반입자료관리_종
ALIAS=DM_반입자료관리_종
CONNECTION_INFO=kolas2@k2up/kolas2#[]
TBL_NAME=IDX_BO_TBL I, BO_SPECIES_TBL S, BO_BOOK_TBL B, BO_ACQ_GROUP_TBL G
CONDITION=I.MANAGE_CODE=UDF_MANAGE_CODE
EXTRA_CONDITION=
#Field Alias|Field Name|Field Type|InitValue|Data Type|
BB_입력일|B.INPUT_DATE|str|||
BB_가격|B.PRICE|int|0||
IBS_저작자|I.AUTHOR_INFO|str|||
IBS_분류표구분|I.CLASS_NO_TYPE|str|||
IBS_형식구분|I.FORM_CODE|str|||
IBS_발행자|I.PUB_INFO|str|||
IBS_발행년|I.PUB_YEAR|str|||
IBS_종KEY|I.REC_KEY|int|||
IBS_본표제|I.TITLE_INFO|str|||
IBS_이용대상구분|I.USE_OBJ_CODE|str|||
BS_MARC|S.MARC|str|||
BS_비고|S.REMARK|str|||
BS_주제구분|S.SUBJECT_CODE|str|||
UDF_책수||str||UDF|
UDF_주제구분설명||str||udf|
UDF_그룹명||str||UDF|
UDF_가격||int||udf|
UDF_화폐구분||str||udf|
UDF_변환가격||str||udf|
IBS_판사항|I.EDIT_INFO|str|||
UDF_표제사항||str||udf|
UDF_총서사항||str||udf|
IBS_매체구분|I.MEDIA_CODE|str|||
IBS_색인별치기호|I.IDX_SE_SHELF_CODE|str|||
IBS_색인등록구분|I.REG_CODE|str|||
IBS_색인편권차|I.IDX_VOL|str|||
IBS_색인권서명|I.IDX_VOL_TITLE|str|||
UDF_일련번호||int||udf|
UDF_발행지||str||udf|
UDF_단가|B.PRICE|int|||
정렬용본표제|I.IDX_TITLE|str|||
IBS_업무구분|I.WORK_CODE|str|||
BB_낱권ISBN|B.EA_ISBN|str|||