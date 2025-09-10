
package ui;

import javax.swing.table.*;
import java.util.*;
import core.ProcessFetcher;

public class ProcessTableModel extends AbstractTableModel {
	private final String[] columns = {"Image Name", "PID", "Mem Usage"};
	private List<ProcessFetcher.ProcessInfo> processes = new ArrayList<>();
	private List<ProcessFetcher.ProcessInfo> filtered = new ArrayList<>();
	private String filter = "";

	public void setProcesses(List<ProcessFetcher.ProcessInfo> list) {
		this.processes = new ArrayList<>(list);
		applyFilter();
	}

	public void setFilter(String filter) {
		this.filter = filter == null ? "" : filter.trim();
		applyFilter();
	}

	private void applyFilter() {
		if (filter.isEmpty()) {
			filtered = new ArrayList<>(processes);
		} else {
			String f = filter.toLowerCase();
			filtered = new ArrayList<>();
			for (ProcessFetcher.ProcessInfo p : processes) {
				if (p.imageName.toLowerCase().contains(f) ||
					String.valueOf(p.pid).contains(f)) {
					filtered.add(p);
				}
			}
		}
		fireTableDataChanged();
	}

	public ProcessFetcher.ProcessInfo getProcessAt(int row) {
		if (row < 0 || row >= filtered.size()) return null;
		return filtered.get(row);
	}

	@Override
	public int getRowCount() {
		return filtered.size();
	}

	@Override
	public int getColumnCount() {
		return columns.length;
	}

	@Override
	public String getColumnName(int col) {
		return columns[col];
	}

	@Override
	public Object getValueAt(int row, int col) {
		ProcessFetcher.ProcessInfo p = filtered.get(row);
		switch (col) {
			case 0: return p.imageName;
			case 1: return p.pid;
			case 2: return p.memUsage;
			default: return null;
		}
	}

	@Override
	public Class<?> getColumnClass(int col) {
		if (col == 1) return Integer.class;
		if (col == 2) return Long.class;
		return String.class;
	}
}