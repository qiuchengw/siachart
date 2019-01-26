-- 全局配置
global={
    font={
        h1="italic-bold 15pt 宋体",
        p1="normal 16px 宋体"
    },
    image={
        img="d:/jpg_small/jpgs/a.jpg"
    },
    color={
        red="#ff0000ff",
        green="#00ff00"
    }
}

-- 窗口应用
paint={
    price={
        background="@global/color/red"
        -- background={
        --     color="@global/red"
        -- }
    },
    title={
        -- 每个窗口可以有自己独立的配置
        common={
            color={
                blue="#0000ffaa"
            }
        },
        background={
            color="@common/blue",
            font="@global/h1"
        }
    },
    ratio={
        background={
            color="@global/green"
        }
    },
    panel={
        background={
            image="@global/img"
        }
    },
    calendar={
        background={
            color="#ff00ff"
        }
    }
}
