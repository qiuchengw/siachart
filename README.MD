# SiaChart
> 可编程，可定制，跨平台，通用的金融图表。

- 内置lua虚拟机，可开发图表内策略
- 跨平台：（linux / windows / mac / android / ios）
- 指标/公式系统
- 外观显示完全可定制

![](header.png)

## 文档
* [SiaChart 中文文档](https://qiuchengw.github.io/zh-cn/)
* [Skia 文档](https://skia.org/)
* [墙内使用skia](https://piaode.ren/2018/08/01/%E6%89%93%E9%80%A0%E8%87%AA%E5%B7%B1%E7%9A%84GUI%EF%BC%9ASkia%E7%BC%96%E8%AF%91/)


## 运行截图

 ![main](https://raw.githubusercontent.com/qiuchengw/siachart/master/imgs/c-main.jpg)

 ![formula](https://raw.githubusercontent.com/qiuchengw/siachart/master/imgs/c-formula.jpg)

  ![style](https://raw.githubusercontent.com/qiuchengw/siachart/master/imgs/c-style.jpg)

## 编译

### 签出必要的源代码

1. Skia签出到“skiall”文件夹内
   ```sh
   git clone --depth=1 https://github.com/google/skia.git skiall
   ```

   墙内码农请使用我提供的gitee镜像：
   ```sh
   git clone --depth=1 -v https://gitee.com/qiuchengw/skiall
   ```

   编译skia有一些坑，请参考我的踩坑文章：[打造自己的GUI：Skia编译](https://piaode.ren/2018/08/01/%E6%89%93%E9%80%A0%E8%87%AA%E5%B7%B1%E7%9A%84GUI%EF%BC%9ASkia%E7%BC%96%E8%AF%91/)

2. 签出SiaChart（本项目）到和skia同目录下
   ```sh
   git clone --depth=1 -v https://gitee.com/qiuchengw/siachart
   ```

### 分平台编译SiaChart：

#### Windows:

```bat
> cd siachart 
> mkdir build & cd build
> cmake -A x64 ..
```
使用visual stuido编译sia.sln解决方案。

#### Linux:

```sh

```

Mac/android/ios 暂不支持。

## Demo
### DemoChart
### Qt Demo

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
