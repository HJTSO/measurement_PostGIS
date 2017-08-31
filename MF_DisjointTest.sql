-----------------------------------------------------------------------
-- Test for Type C. MF_Disjoint
-----------------------------------------------------------------------
DO $$
DECLARE
    test_case text;
    tr MF_Trajectory;
    another_tr MF_Trajectory;
    r boolean;
	
BEGIN
    -------------------------------------------------------------------
    -- 準備
    -------------------------------------------------------------------
    tr = MF_TrajectoryFromGeometry(
        ST_GeomFromText('LINESTRING(0 200, 100 100, 200 100, 300 200)'),
        timestamp '2017-08-01 10:00:00',
        timestamp '2017-08-01 11:00:00');
    another_tr = MF_TrajectoryFromGeometry(
        ST_GeomFromText('LINESTRING(0 0, 100 100, 200 100, 300 0)'),
        timestamp '2017-08-01 10:00:00',
        timestamp '2017-08-01 11:00:00');
		
	-- ST_AsText — ジオメトリ/ジオグラフィのSRIDメタデータのないWell-Known Text (WKT)表現を返します
    --RAISE INFO 'tr = %', ST_AsText(tr);
    --RAISE INFO 'another_tr = %', ST_AsText(another_tr);

    RAISE INFO '--------------------------------------------------------------------------------';
    test_case = '重なっていない期間を含めて指定';
    RAISE INFO '%', test_case;

    --   s      e
    --    \    /
    --     ====
    --    /    \
    --   s      e
    --  <->

    r = MF_Disjoint(tr, another_tr,
        timestamp '2017-08-01 10:00:00',
        timestamp '2017-08-01 10:15:00');
    RAISE INFO '結果: %', r;
    ASSERT r IS NOT NULL,
        '結果がNULLでないべき';
    ASSERT r = TRUE,
        '結果がTRUEであるべき';

    RAISE INFO '% ... 成功', test_case;

    RAISE INFO '--------------------------------------------------------------------------------';
    test_case = '重なっている期間のみを指定';
    RAISE INFO '%', test_case;

    -- s      e
    --  \    /
    --   ====
    --  /    \
    -- s      e
    --   <-->

    r = MF_Disjoint(tr, another_tr,
        timestamp '2017-08-01 10:25:00',
        timestamp '2017-08-01 10:35:00');
    RAISE INFO '結果: %', r;
    ASSERT r IS NOT NULL,
        '結果がNULLでないべき';
    ASSERT r = FALSE,
        '結果がFALSEであるべき';

    RAISE INFO '% ... 成功', test_case;

    RAISE INFO '--------------------------------------------------------------------------------';
    test_case = '重なっている期間のみを指定';
    RAISE INFO '%', test_case;

    -- s      e
    --  \    /
    --   ====
    --  /    \
    -- e      s **始終端の時間が逆**
    --   <-->

    tr = MF_TrajectoryFromGeometry(
        ST_GeomFromText('LINESTRING(0 200, 100 100, 200 100, 300 200)'),
        timestamp '2017-08-01 10:00:00',
        timestamp '2017-08-01 11:00:00');
    another_tr = MF_TrajectoryFromGeometry(
        ST_GeomFromText('LINESTRING(300 0, 200 100, 100 100, 0 0)'),
        timestamp '2017-08-01 10:00:00',
        timestamp '2017-08-01 11:00:00');

    r = MF_Disjoint(tr, another_tr,
        timestamp '2017-08-01 10:25:00',
        timestamp '2017-08-01 10:35:00');
    RAISE INFO '結果: %', r;
    ASSERT r IS NOT NULL,
        '結果がNULLでないべき';
    ASSERT r = FALSE,
        '結果がFALSEであるべき';

    RAISE INFO '% ... 成功', test_case;
	
	RAISE INFO '--------------------------------------------------------------------------------';
    test_case = '重なっていない期間を含めて指定';
    RAISE INFO '%', test_case;

    --   s      e
    --    \    /
    --     ====
    --    /    \
    --   s      e
    --  <->    <-> **始終端の時間が逆**

    r = MF_Disjoint(tr, another_tr,
        timestamp '2017-08-01 10:00:00',
        timestamp '2017-08-01 10:15:00');
    RAISE INFO '結果: %', r;
    ASSERT r IS NOT NULL,
        '結果がNULLでないべき';
    ASSERT r = TRUE,
        '結果がTRUEであるべき';

    RAISE INFO '% ... 成功', test_case;

END
$$ LANGUAGE plpgsql;
