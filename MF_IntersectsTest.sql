-----------------------------------------------------------------------
-- Test for Type C. MF_Intersects
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
        ST_GeomFromText('LINESTRING(0 200 100, 100 100 0, 200 100 0, 300 200 -100)'),
        timestamp '2017-08-01 10:00:00',
        timestamp '2017-08-01 11:00:00');
    another_tr = MF_TrajectoryFromGeometry(
        ST_GeomFromText('LINESTRING(0 0 -100, 100 100 0, 200 100 0, 300 0 100)'),
        timestamp '2017-08-01 10:00:00',
        timestamp '2017-08-01 11:00:00');

    RAISE INFO '--------------------------------------------------------------------------------';
    test_case = '重なっていない期間を含めて指定';
    RAISE INFO '%', test_case;

    -- s      e
    --  \    /
    --   ====
    --  /    \
    -- s      e
    -- <------>

    r = MF_Intersects(tr, another_tr,
        timestamp '2017-08-01 10:00:00',
        timestamp '2017-08-01 11:00:00');
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

    r = MF_Intersects(tr, another_tr,
        timestamp '2017-08-01 10:25:00',
        timestamp '2017-08-01 10:35:00');
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
    -- e      s **始終端の時間が逆**
    --   <-->

    tr = MF_TrajectoryFromGeometry(
        ST_GeomFromText('LINESTRING(0 200 100, 100 100 0, 200 100 0, 300 200 -100)'),
        timestamp '2017-08-01 10:00:00',
        timestamp '2017-08-01 11:00:00');
    another_tr = MF_TrajectoryFromGeometry(
        ST_GeomFromText('LINESTRING(300 0 100, 200 100 0, 100 100 0, 0 0 -100)'),
        timestamp '2017-08-01 10:00:00',
        timestamp '2017-08-01 11:00:00');

    r = MF_Intersects(tr, another_tr,
        timestamp '2017-08-01 10:25:00',
        timestamp '2017-08-01 10:35:00');
    RAISE INFO '結果: %', r;
    ASSERT r IS NOT NULL,
        '結果がNULLでないべき';
    ASSERT r = TRUE,
        '結果がTRUEであるべき';

    RAISE INFO '% ... 成功', test_case;

END
$$ LANGUAGE plpgsql;
