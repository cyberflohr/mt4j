package de.flohrit.mt4j;

import java.util.ArrayList;
import java.util.List;

public abstract class AbstractBasicClient implements MT4BasicClient {

	private List<Double> high = new ArrayList<Double>();
	private List<Double> low = new ArrayList<Double>();
	private List<Double> open = new ArrayList<Double>();
	private List<Double> close = new ArrayList<Double>();
	private double bid;
	private double ask;
	private int maxBars = 100;
	
	@Override
	public int processTick(double bid, double ask) {
		return PROCESS_TICK_NONE;
	}

	@Override
	public void addNewBar(double high, double low, double open, double close) {
		trimArray(this.high);
		this.high.add(0, high);
		
		trimArray(this.low);
		this.low.add(0, low);

		trimArray(this.open);
		this.open.add(0, open);
		
		trimArray(this.close);
		this.close.add(0, close);
	}
	
	/**
	 * @return last bars high value
	 */
	public double getHigh() {
		return high.get(0);
	}

	/**
	 * @param idx bar number (0 == last bar) 
	 * @return high value of bar
	 */
	public double getHigh(int idx) {
		return high.get(idx);
	}

	/**
	 * @return last bars low value
	 */
	public double getLow() {
		return low.get(0);
	}

	/**
	 * @param idx bar number (0 == last bar) 
	 * @return low value of bar
	 */
	public double getLow(int idx) {
		return low.get(idx);
	}

	/**
	 * @return last bars high value
	 */
	public double getOpen() {
		return open.get(0);
	}

	/**
	 * @param idx bar number (0 == last bar) 
	 * @return open value of bar
	 */
	public double getOpen(int idx) {
		return open.get(idx);
	}

	/**
	 * @return last bars high value
	 */
	public double getClose() {
		return close.get(0);
	}

	/**
	 * @param idx bar number (0 == last bar) 
	 * @return close value of bar
	 */
	public double getClose(int idx) {
		return close.get(idx);
	}

	/**
	 * @return last bid price
	 */
	public double getBid() {
		return bid;
	}

	/**
	 * @return last ask price
	 */
	public double getAsk() {
		return ask;
	}
	
	public void setMaxBars(int maxBars) {
		this.maxBars = maxBars;
	}
	
	private void trimArray(List<Double> values) {
		for (;values.size() >= maxBars;) {
			values.remove(values.size()-1);
		}
	}
}
