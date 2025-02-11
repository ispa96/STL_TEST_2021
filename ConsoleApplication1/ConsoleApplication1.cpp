/**


**/


#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

#define FAST std::ios_base::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0)
#define INF 1e9

std::ifstream fin("input.in");
std::ofstream fout("output.txt");

using ll = long long;

class PlannedEvent {
public:
	std::string m_name;
	int m_planningDay, m_startingHour, m_duration;

public:
	PlannedEvent() {
		m_name = "";
		m_planningDay = 0;
		m_startingHour = 0;
		m_duration = 0;
	}
};

class IncomingEvent {
public:
	std::string m_name;
	int m_duration, m_priority;

public:
	IncomingEvent() {
		m_name = "";
		m_duration = 0;
		m_priority = 0;
	}

	bool operator<(const IncomingEvent& other) const {
		if (m_priority != other.m_priority)
			return m_priority > other.m_priority;
		return m_duration < other.m_duration;
	}
};

std::ostream& operator<<(std::ostream& COUT, const PlannedEvent& ev) {
	COUT << ev.m_name << ' ' << ev.m_planningDay << ' ' << ev.m_startingHour << ' ' << ev.m_duration;
	return COUT;
}

std::ostream& operator<<(std::ostream& COUT, const IncomingEvent& ev) {
	COUT << ev.m_name << ' ' << ev.m_duration << ' ' << ev.m_priority;
	return COUT;
}


template<typename T> static void showVector(const std::vector<T>& v) {
	for (const auto& i : v)
		fout << i << '\n';
	fout << '\n';
}

static void solve() {
	int noDays, noPlannedEvents, noIncomingEvents, firstIncomingEventDay = 0, firstIncomingEventHour = 0;
	std::vector<PlannedEvent> plannedEvents;
	std::vector<IncomingEvent> incomingEvents, plannedIncomingEvents;
	std::map<int, std::vector<std::pair<int, int>>> mp;
	std::map<IncomingEvent, int> plannedDay, plannedHour;

	fin >> noDays;

	for (int i = 1; i <= noDays; i++)
		mp[i].emplace_back(std::make_pair(9, 9));

	fin >> noPlannedEvents;
	plannedEvents.resize(noPlannedEvents);

	for (int i = 0; i < noPlannedEvents; i++)
		fin >> plannedEvents[i].m_name >> plannedEvents[i].m_planningDay >> plannedEvents[i].m_startingHour >> plannedEvents[i].m_duration;

	fin >> noIncomingEvents;
	incomingEvents.resize(noIncomingEvents);

	for (int i = 0; i < noIncomingEvents; i++)
		fin >> incomingEvents[i].m_name >> incomingEvents[i].m_duration >> incomingEvents[i].m_priority;

	/*fout << "-> planned events:\n";
	showVector<PlannedEvent>(plannedEvents);
	fout << "\n-> incoming events:\n";
	showVector<IncomingEvent>(incomingEvents);*/

	/// le sortam pe cele planificate, in functie de ora de start
	sort(plannedEvents.begin(), plannedEvents.end(), [](const PlannedEvent& e1, const PlannedEvent& e2) {
		return e1.m_startingHour < e2.m_startingHour;
		});

	// showVector<PlannedEvent>(plannedEvents);

	/// pune la fiecare zi perechea de evenimente (inceput, sfarsit)
	for (const auto& ev : plannedEvents)
		mp[ev.m_planningDay].emplace_back(std::make_pair(ev.m_startingHour, ev.m_startingHour + ev.m_duration));

	for (int i = 1; i <= noDays; i++)
		mp[i].emplace_back(std::make_pair(17, 17));

	/*for (const auto& i : mp) {
		fout << i.first << ": ";

		for (const auto& p : mp[i.first])
			fout << '(' << p.first << ", " << p.second << ") ";
		fout << '\n';
	}*/

	sort(incomingEvents.begin(), incomingEvents.end(), [](const IncomingEvent& e1, const IncomingEvent& e2) {
		return e1.m_priority > e2.m_priority;
		});

	// showVector<IncomingEvent>(incomingEvents);
	bool ok = false;

	for (const auto& it : mp) {
		int day = it.first;
		auto& v = it.second;

		for (int i = 1; i < v.size(); i++) {
			if (v[i].first - v[i - 1].second >= incomingEvents[0].m_duration) {
				firstIncomingEventDay = day;
				firstIncomingEventHour = v[i - 1].second;
				ok = true;
			}

			if (ok)
				break;
		}

		if (ok)
			break;
	}

	fout << firstIncomingEventDay << ' ' << firstIncomingEventHour << '\n';

	for (const auto& incomingEvent : incomingEvents) {
		bool planned = false;

		for (auto& it : mp) {
			for (int i = 1; i < it.second.size(); i++) {
				if (it.second[i].first - it.second[i - 1].second >= incomingEvent.m_duration) {
					planned = true;

					plannedIncomingEvents.emplace_back(incomingEvent);
					plannedDay[incomingEvent] = it.first;
					plannedHour[incomingEvent] = it.second[i - 1].second;

					it.second.emplace_back(std::make_pair(it.second[i - 1].second, it.second[i - 1].second + incomingEvent.m_duration));
					sort(it.second.begin(), it.second.end());
				}

				if (planned)
					break;
			}

			if (planned)
				break;
		}

		/*mp[plannedDay[incomingEvent]].emplace_back(std::make_pair(plannedHour, plannedHour[incomingEvent] + incomingEvent.m_duration));
		sort(mp[plannedDay[incomingEvent]].begin(), mp[plannedDay[incomingEvent]].end());*/
	}

	sort(plannedIncomingEvents.begin(), plannedIncomingEvents.end(), [&plannedDay](const IncomingEvent& e1, const IncomingEvent& e2) {
		return plannedDay[e1] < plannedDay[e2];
		});

	fout << plannedIncomingEvents.size() << '\n';
	for (const auto& e : plannedIncomingEvents)
		fout << e.m_name << ' ' << plannedDay[e] << ' ' << plannedHour[e] << '\n';
}

int main() {
	FAST;

	solve();

	return 0;
}


/**

	EX:

	-> clasa pentru eveniment

	-> avem un vector de zile (n zile), pentru fiecare zi(posibil clasa), avem un vector de pair, ce ne sugereaza intervalele orare, cand sunt planificate evenimentele
	-> ordonam acest vector dupa primul element din pair
	-> sau putem avea o mapa de int si vector de pair

	-> ordonam evenimentele ce trebuie planificate dupa prioritate, apoi ce ne mai spune in enunt

**/