# SiaChart
> 可编程，可定制，跨平台，通用的金融图表。

- 内置lua虚拟机，可开发图表内策略
- 跨平台：（linux / windows / mac / android / ios）
- 指标/公式系统
- 外观显示完全可定制

![](header.png)

## 开发进度
### 平台
**桌面**
 - [x] Windows
 - [x] Linux
 - [ ] Mac
  
**移动端**
 - [ ] Android
 - [ ] iOS
  
### 功能

**图表基础**
 - [x] 数据源自动排序
 - [x] 显示精度调整接口
 - [x] 十字线
 - [x] 启用/禁用内建功能接口
 - [x] Tooltip/Tag
 - [x] 指标公式系统
 - [x] 面板网格线
 - [x] 定制外观显示接口

**序列支持情况**
- [x] Candle
- [x] Line
- [x] Bar
- [x] Updown
- [ ] 更多序列类型支持
- [x] 序列的外观设置
- [x] 序列统一开发接口

**指标（公式）**
- [x] 集成Ta-lib
- [ ] 指标公式统一开发接口

**自定义对象（Object）**
- [x] 对象的定位
- [x] 文本
- [x] 图片
- [ ] 富文本排版
	
**脚本引擎（Lua）**
- [x] 内嵌Lua引擎
- [x] 外部配置文件解析
- [ ] 使用Lua开发公式/指标


## 文档

**SiaChart文档**
> * [SiaChart 中文文档](https://qiuchengw.github.io/zh-cn/)

**其他相关文档**
> * [Skia 文档](https://skia.org/)
> * [墙内使用skia](https://piaode.ren/2018/08/01/%E6%89%93%E9%80%A0%E8%87%AA%E5%B7%B1%E7%9A%84GUI%EF%BC%9ASkia%E7%BC%96%E8%AF%91/)


## 运行截图

 ![main](https://raw.githubusercontent.com/qiuchengw/siachart/master/imgs/c-main.png)

<table>
	<tr>
		<td><img src="https://raw.githubusercontent.com/qiuchengw/siachart/master/imgs/c-formula.jpg" width="330" height="650" /></td>
		<td><img src="https://raw.githubusercontent.com/qiuchengw/siachart/master/imgs/c-style.png" width="330" height="650" /></td>
	</tr>
</table>

## 编译 & 运行

请参考wiki：[编译](https://github.com/qiuchengw/siachart/wiki/%E7%BC%96%E8%AF%91)


## Demo
### DemoChart（基本功能展示）

请参考wiki：[基础Demo](https://github.com/qiuchengw/siachart/wiki/Simple-Demo)

### Qt Demo（全功能展示）

请参考wiki：[Qt Demo build & run](https://github.com/qiuchengw/siachart/wiki/Qt-Demo---build-&-run)

## 版本历史

暂无
<!-- * 0.0.1 -->
<!-- * Work in progress -->

## 版权

Distributed under the GPL license. See ``LICENSE`` for more information.

## 贡献

1. Fork it (<https://github.com/qiuchengw/siachart/fork>)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request

## 关于

> 作者：[qiuchengw - 漂的人](https://piaode.ren) 
> 
> 微信：qiuchengw （欢迎技术交流） 
> 
> ![me-wexin](https://raw.githubusercontent.com/qiuchengw/siachart/master/imgs/wx-me.jpg)
> 
> 邮箱：qiuchengw@qq.com

## 赞助：

Buy me a bottle of water!

 ![buy me a bottle of water](https://raw.githubusercontent.com/qiuchengw/siachart/master/imgs/wxpay.jpg)
