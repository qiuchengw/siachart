-- 全局配置
global={
    font={
        h1="italic-bold 15pt 宋体",
        p1="normal 16px 宋体",
        b1="bold 16 宋体"
    },
    image={
        img="d:/jpg_small/jpgs/a.jpg"
    },
    color={
        theme="#000000",
        red="#ff0000",
        green="#00ff00",
        border="#ff0000ff",
        inverse="#ffffff"
    },
    stroke={
        border="2 round miter"
    },
    effect={
        dashed="dashed:10.0 4.0 2.0 5.0",
        dotted="dashed:1.0 4.0"
    }
}

-- 窗口应用
paint={
    mainview={
        background={
            color="@global/color/theme"
        },
        grid={
            color="#930a01",
            stroke="1 square miter",
            effect="@global/dashed"
        }
        -- ,
        -- border={
        --     color="@global/theme",
        --     stroke="1 square miter"
        -- }
    },
    title={
        background={
            color="@global/theme"
        },
        text={
            font="bold 13pt 宋体",
            color="#ffffffff"
        },
        border={
            stroke="1 square miter",
            color="@global/border"
        },
        geometry=",,,20"
    },
    ratio_axis={
        background={
            color="@global/theme"
        },
        positive={
            color="#ff0000ff"
        },
        negative={
            color="#00ff00ff"
        },
        border={
            color="@global/border",
            stroke="1 square miter"
        },
        geometry=",,60,"
    },
    value_axis={
        background={
            color="@global/theme"
        },
        border={
            color="@global/border",
            stroke="1 square miter"
        },
        geometry=",,60,"
    },
    calendar={
        background={
            color="@global/theme"
        },
        border={
            stroke="@global/border",
            color="@global/border"
        },
        geometry=",,,20"
    },
    crosshair={
        line={
            color="@global/inverse",
            stroke="1 square miter"
        },
        tag_background={
            color="#0000ff"
        },
        tag_border={
            stroke="1",
            color="@global/border"
        },
        tag_text={ 
            color="#ffffff",
            font="@global/font/b1"
        },
        ---- detail panel
        detail_border={
            stroke="2 square miter",
            color="#ffffff"
        },
        detail_background={
            color="#000000"
        },
        detail_title={
            font="normal",
            color="#ffffff"
        },
        detail_value={
            color="#54FFFF"
        },
        geometry=",,60,",
        -- rubber
        rubber={
            color="#9EEA6AAA"
        }
    },
    tooltip={
        background={
            color="#101042"
        },
        border={
            stroke="1",
            color="#E7C610"
        },
        text={
            color="#ffffff"
        }
    }
}
