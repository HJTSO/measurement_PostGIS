-----------------------------------------------------------------------
-- Type C. MF_Intersects
-----------------------------------------------------------------------
-- 指定された期間の間に、指定された軌跡と（時刻含め）交差している場合、trueを返す。ST_3DIntersects
-- パラメータ "timeInterval"は、検索を特定の期間に制限するものとする。
--
-- パラメータ:
-- - MF_Trajectory (TemporalTrajectory): 比較元の軌跡
-- - MF_Trajectory (TemporalTrajectory): 比較先の軌跡
-- - timeInterval: TemporalPeriod: 期間
-- 戻り値:
-- - boolean (Boolean): 判定結果

CREATE OR REPLACE FUNCTION MF_Intersects(
    tr MF_Trajectory,
    another_tr MF_Trajectory,
    interval_from timestamp,
    interval_to timestamp)
    -- TODO 引数の期間、個別? 配列? ※要検討
RETURNS boolean AS $$

DECLARE
    tr_src MF_Trajectory;
    tr_dest MF_Trajectory;
	
BEGIN

    -- 期間で分割
    -- TODO MF_SubTrajectory が from to null 許容するかどうか検討後修正。
    tr_src = MF_SubTrajectory(tr,
         interval_from, interval_to);
    tr_dest = MF_SubTrajectory(another_tr,
         interval_from, interval_to);

    IF tr_src = NULL OR tr_dest = NULL THEN
        RETURN NULL;
    END IF;
	
	-- ST_3DIntersects — ジオメトリが3次元で「空間的にインタセクトする」場合にTRUEを返します。
	--                   ポイント、ラインストリング、ポリゴン、多面体サーフェス 
    -- boolean ST_3DIntersects( geometry geomA , geometry geomB );
	-- http://cse.naro.affrc.go.jp/yellow/pgisman/2.3.0/ST_3DIntersects.html
	-- http://postgis.net/docs/ST_3DIntersects.html
	RETURN ST_3DIntersects(tr_src.line_string, tr_dest.line_string);
END;
$$ LANGUAGE plpgsql;
