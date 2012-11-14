package de.flohrit.mt4j;

/**
 * Simple trading interface between MT4 and Java. 
 * @author wflohr
 */
public interface MT4BasicClient {

	/**
	 * No action should be taken by the MT4 client after processTick has finished
	 */
	public static int PROCESS_TICK_NONE = 0;

	/**
	 * A buy action should be executed by the MT4 client after processTick has finished
	 */
	public static int PROCESS_TICK_DO_BUY = 1;

	/**
	 * A sell action should be executed by the MT4 client after processTick has finished
	 */
	public static int PROCESS_TICK_DO_SELL = 2;

	/**
	 * Called by the MT4 client on each new tick. 
	 * @param bid last bid price
	 * @param ask last ask price
	 * @return <code>PROCESS_TICK_NONE</code>, <code>PROCESS_TICK_DO_BUY</code> or <code>PROCESS_TICK_DO_SELL</code>
	 */
	public int processTick(double bid, double ask);

	/**
	 * Called by the MT4 client when a new bar was created
	 * @param high last bar high price
	 * @param low last bat low price
	 * @param open last bar open price
	 * @param close last bar close price
	 */
	public void addNewBar(double high, double low, double open, double close);
	
	/**
	 * Sets how many bars are kept in memory (default=100)
	 * @param numBars number of bars (bar historie) keept in memory
	 */
	public void setMaxBars(int numBars); 
	
	/**
	 * Called by the MT4 client during init phase.
	 */
	public void init();
	
	/**
	 * Called by the MT4 client during deinit phase.
	 */
	public void deinit();
}
