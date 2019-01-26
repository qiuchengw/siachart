-- paint 是从1个或多个资源(res)上建立而来
-- 所以在执行解析paint前，应该先解析资源

-- paint 是从1个或多个资源上建立而来
-- item: @paint/group/item
function build_paint_item(item, ress)
    if type(ress)=='table' then
        -- 使用的是table形式描述
        for k, v in pairs(ress) do
            -- 这儿要陷入到宿主程序中去执行paint的创建
            build_paint(item, k, v)
        end
    elseif type(ress)=='string' then
        -- 使用字符串形式描述painter
        build_paint(item, '', ress)
    end
end

-- 解析一个窗口项
function parse_paint_item(group, tbl)
    -- 窗口也可以有自己的一个独立配置
    -- 如果有自己的配置的话，那么我们就必须先解析这个，因为下面会引用它
    if tbl['common'] ~= nil then
        if not parse_common(group, tbl['common']) then
            return false
        end

        -- 解析完就删掉，
        -- 这样删除，并没有减少tbl的长度，下面的循环将会重复解析common下面的一个项目两次
        -- FIXME: 有效的删除方法
        tbl['common'] = nil
    end

    -- 接下来，再解析非通用的
    for k, v in pairs(tbl) do
        build_paint_item(group.."/"..k, v)
    end
end

-- 解析窗口自己的应用
function parse_paint()
    if nil == paint then
        parse_error("paint not defined!")
        return
    end

    -- 每个字段都必须是table类型！
    for k, v in pairs(paint) do
        if type(v) ~= 'table' then
            parse_error("error: lua paint field must be table!")
            return
        end
        -- 解析一个字段
        parse_paint_item("@paint/"..k, v)   
    end
end

-- 将这部分资源
parse_common('@global', global)
parse_paint()