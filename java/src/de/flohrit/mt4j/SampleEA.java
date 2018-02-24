package de.flohrit.mt4j;

import java.io.FileNotFoundException;
import java.io.PrintStream;

public class SampleEA extends AbstractBasicClient {
	
	private int intPropertySample;
	private double doublePropertySample;
	private boolean boolPropertySample;
	private String stringPropertySample;
	
	public  SampleEA() {
		super();
	}

	public int getIntPropertySample() {
		return intPropertySample;
	}

	public void setIntPropertySample(int intProperty) {
		this.intPropertySample = intProperty;
	}

	public double getDoublePropertySample() {
		return doublePropertySample;
	}

	public void setDoublePropertySample(double doubleProperty) {
		this.doublePropertySample = doubleProperty;
	}

	public boolean getBoolPropertySample() {
		return boolPropertySample;
	}

	public void setBoolPropertySample(boolean boolProperty) {
		this.boolPropertySample = boolProperty;
	}
	
	public String getStringPropertySample() {
		return stringPropertySample;
	}

	public void setStringPropertySample(String stringProperty) {
		this.stringPropertySample = stringProperty;
	}

	@Override
	public int processTick(double bid, double ask) {
		super.processTick(bid, ask);
		return PROCESS_TICK_NONE;
	}

	@Override
	public void addNewBar(double high, double low, double open, double close) {
		super.addNewBar(high, low, open, close);
	}

	@Override
	public void init() {
		System.out.println("init(1)");
	}

	@Override
	public void deinit() {
		System.out.println("deinit(9)");
	}

	public void doSomething() {
		System.out.println("doSomething()");
	}
	
	static {
		try {
			System.setOut(new PrintStream("experts/logs/SampleEA.log"));
			System.setErr(new PrintStream("experts/logs/SampleEA_error.log"));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}
}
