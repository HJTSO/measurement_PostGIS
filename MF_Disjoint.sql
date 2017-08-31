-----------------------------------------------------------------------
-- Type C. MF_Disjoint
-----------------------------------------------------------------------
-- 指定された期間の間ずっと、指定された軌跡と（時刻含め）交差していない場合、trueを返す。
-- パラメータ "timeInterval"は、検索を特定の期間に制限するものとする。
--
-- パラメータ:
-- - MF_Trajectory (TemporalTrajectory): 比較元の軌跡
-- - MF_Trajectory (TemporalTrajectory): 比較先の軌跡
-- - timeInterval: TemporalPeriod: 期間
-- 戻り値:
-- - boolean (Boolean): 判定結果

CREATE OR REPLACE FUNCTION MF_Disjoint(
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

	-- ST_Disjoint — ジオメトリが「空間的にインタセクトし」ていない、すなわち、空間を共有していない場合に、TRUEを返します。
	-- boolean ST_Disjoint( geometry A , geometry B );
	-- http://cse.naro.affrc.go.jp/yellow/pgisman/2.3.0/ST_Disjoint.html
	-- http://postgis.net/docs/ST_Disjoint.html
    RETURN ST_Disjoint(tr_src.line_string, tr_dest.line_string);
END;
$$ LANGUAGE plpgsql;
