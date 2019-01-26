-- 通用资源的解析
-- 资源的格式如下：
---------------------------------
-- global={
--     font={
--         h1="italic-bold 15pt 宋体",
--         p1="normal 16px 宋体"
--     },
--     image={
--         img="d:/jpg_small/jpgs/a.jpg"
--     },
--     color={
--         red="#ff0000ff",
--         green="#00ff00"
--     }
-- }
------------------------------------

function host_error(str)
    host_reg_res(str)
end

-- 解析某一类的资源配置
function parse_common_item(namespace, typ, tbl)
    for k,v in pairs(tbl) do
        if type(v) ~= 'string' then
            host_error("resource val must be string!")
            return false
        end
    
        -- print("---> parse_res_item:", namespace, "  ", typ, "  ", k, "  ", v)
        -- 这个地方要陷入到host程序中去添加资源。lua本身是不用这些资源的
        host_reg_res(namespace, typ, k, v)
    end
    return true
end

-- 解析通用的配置
function parse_common(namespace, tbl)
    if type(tbl) ~= 'table' then
        return false
    end

    for k,v in pairs(tbl) do
        if type(v)=='table' then
            parse_common_item(namespace, k, v)
        end
    end
    return true
end

