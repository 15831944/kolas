#[Data Manager]60182706 ▶ DM_연속_수서_통계_등록구분
ALIAS=DM_연속_수서_통계_등록구분
CONNECTION_INFO=kolas2@k2up/kolas2#[]
TBL_NAME=idx_se_tbl i, se_vol_tbl v,se_book_tbl b
CONDITION=
EXTRA_CONDITION=group by b.reg_code
#Field Alias|Field Name|Field Type|InitValue|Data Type|
종|count(distinct(i.rec_key))|int|||
권|count(distinct(v.rec_key))|int|||
등록구분|b.reg_code|str|||
책|count(distinct(b.rec_key))|int|||
