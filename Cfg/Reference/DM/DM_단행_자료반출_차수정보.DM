#[Data Manager]100000000 ▶ DM_단행_자료반출_차수정보
ALIAS=DM_단행_자료반출_차수정보
CONNECTION_INFO=kolas2@k2up/kolas2#[]
TBL_NAME=BO_PURCHASE_SEQ_NO_TBL Q
CONDITION=Q.MANAGE_CODE=UDF_MANAGE_CODE
EXTRA_CONDITION=
#Field Alias|Field Name|Field Type|InitValue|Data Type|
BQ_REC_KEY|Q.REC_KEY|int|||
BQ_수입년도|Q.ACQ_YEAR|int|||
BQ_업무구분|Q.WORK_CODE|str|||
BQ_차수번호|Q.SEQ_NO|int|||
BQ_작업번호사용여부|Q.WORK_NO_USE_YN|str|||
BQ_차수내마지막작업번호|Q.LAST_WORK_NO|int|||
BQ_차수생성일|Q.SEQ_NO_CREATE_DATE|str|||
BQ_차수생성자|Q.SEQ_NO_CREATE_WORKER|str|||
BQ_중복조사여부|Q.COPY_CHECK_YN|str|||
BQ_복본조사여부|Q.DUP_CHECK_YN|str|||
BQ_심의완료여부|Q.REVIEW_CHECK_YN|str|||
BQ_주문일자|Q.ORDER_DATE|str|||
BQ_검수완료일자|Q.CHECKIN_COMPLETE_DATE|str|||
BQ_작업상태|Q.WORKING_STATUS|str|||
BQ_최초작업|Q.FIRST_WORK|str|||
BQ_마지막작업|Q.LAST_WORK|str|||
BQ_비고|Q.REMARK|str|||
