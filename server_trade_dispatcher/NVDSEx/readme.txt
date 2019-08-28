必须重载的类
	CClientMacCollection			//机器集合，每一种游戏有其对应的一个机器集合对象。
		必须重写的接口:
		1、int dispatchOrder(CTradeOrder* order) 
				分派一个订单到两个交易客户端【当允许躲开时可能是一个】
		2、void OrderProcessed(const string& _order_id,int client_mac_id,int result_id)
				订单处理完毕或者发生错误交易过程被中断，返回结果result_id。
		
	CTradeOrderCollection			//订单集合，每一种游戏有其对应的一种订单集合对象。
	
/**
 * 交易进度描述：
 * ①收到了订单信息并已分配好两边的机器						-- 初始状态
 * ②订单已经已经发到seller客户端 | 订单已经发到buyer客户端	-- 等待客户端启动，进入角色进入状态③
 * ③seller进入角色成功、失败 | buyer进入角色 成功、失败号	-- 等待两边都进入角色成功后进入状态④
 * ④seller角色已经在指定场景 | buyer角色已经在指定场景		-- 服务器发出允许交易的命令
 * ⑤正在交易												-- 等待交易结果
 * ⑥交易成功、失败											-- 交易完成，服务器发还确认信息后客户端退出
 */
enum EOrder_progress_in_mac{
	EOP_standby,					//待命，准备接收账号密码
	EOP_order_received,				//客户端收到了账号，密码等信息
	EOP_in_game_role,				//客户端进入了游戏角色
	EOP_at_trade_stage,				//角色进入了指定场景
	EOP_ready_to_trade,				//客户端已经准备好交易 -- 只差双方按下交易按钮，等待服务器允许交易的命令。
	EOP_trading,					//正在交易
	EOP_trade_end,					//交易完成
};

订单派发器中的订单状态
	①从数据库取出						--  为初始状态，取出的账号放到待处理列表中交易机有请求时发给交易机派发器转为状态②
	②已发给交易机派发器				--  交易完成后转为状态③
	③订单处理完毕-成功、错误号			--	保存结果后转为状态④
	④订单处理结果已保存到服务器		--  该订单将被删除


关闭订单派发器的具体步骤
	第一步：设置STOP标志禁止继续派发订单
	第二步：在列表中找出仍为状态①的订单存回数据库，然后从列表删除
	第三步：等待状态②的订单转为状态③<服务器已经授权交易将等待交易完成>或者转为状态①<已派发但尚未授权交易将取消交易>。
	重复第二步和第三步直到列表为空。第三步中状态②到状态③的订单将按照正常过程处理。
	
	
	/**
	 * 交易完成后的处理工作，根据OrderId查找出CTradeOrderProcessInfo，然后找出交易双方的机器。
	 * 默认情况下：
	 * 1、删除TradeClientMachine的CTradeOrder列表中对应的CTradeOrder对象
	 * 2、更新TradeClientMachine的负载、交易记录。
	 * 3、将交易订单处理信息（CTradeOrderProcessInfo对象）[包含订单、时间和机器信息]写入数据库（或记录文件）。
	 *    并删除该order对应的CTradeOrderProcessInfo对象。
	 * 4、向OrderDispatcher报告订单完成情况【由orderDispatcher最终处理CTradeOrder】。
	 *    注意在此步骤后，CTradeOrderProcessInfo和TradeClientMachine中引用的该CTradeOrder对象不再是可靠地对象。
	 *    它可能被删除，或者移动到其他地方。
	 */
	 
	 
订单在收到buddyfailed后如何保存进数据库，如果原来已经有了result值是否应该覆盖之？

在记录中有app值的原因是因为该订单经过多次处理（至少2此），服务器在关闭时该订单被回收,且该订单第一次没有调用saveOrderLog。