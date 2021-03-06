#[Data Manager]62984679 ▶ DM_BO_LOC_3822
ALIAS=DM_BO_LOC_3822
CONNECTION_INFO=kolas2@k2up/kolas2#[]
TBL_NAME=MN_MAIL_TBL M, MN_MAIL_GROUP_TBL G
CONDITION=M.group_key = G.rec_key and G.m_type='열람' AND G.MANAGE_CODE=UDF_MANAGE_CODE
EXTRA_CONDITION=order by to_number(M.mail_class) asc
#Field Alias|Field Name|Field Type|InitValue|Data Type|
메일종류|G.mng_group|str|||
메일클래스|M.MAIL_CLASS|str|||
메일KEY|M.REC_KEY|int|||
기능사용여부|M.USE_YN|str|||
제목|M.TITLE|str|||
본문|M.CONTENTS|str|||
서문|M.CONTENTS_HEADER|str|||
결문|M.CONTENTS_TAIL|str|||
