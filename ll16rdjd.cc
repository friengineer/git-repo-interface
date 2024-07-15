#include "globals.h"
#include "gitpp.h"
#include <QtWidgets>

namespace {
	GITPP::REPO r("..");

	// class definition for a branch tab
	class BranchesTab : public QWidget {
			Q_OBJECT

		public:
			BranchesTab();

		private:
			void createWidgets();
			void arrangeWidgets();
			void makeConnections();

			QLabel* titleLabel;
			QLabel* mainText;
			QLabel* branchLabel;
			QComboBox* branchSelector;
			QPushButton* selectButton;
			QMessageBox* switched;
			QMessageBox* error;

		private slots:
			void switchBranch();
	};

	// constructor
	BranchesTab::BranchesTab() {
		createWidgets();
		arrangeWidgets();
		makeConnections();
	}

	// creates the widgets for the tab
	void BranchesTab::createWidgets() {
		titleLabel = new QLabel("Branches");
		mainText = new QLabel("Select which branch you would like to view.");

		branchLabel = new QLabel("Branch: ");
		branchSelector = new QComboBox();
		selectButton = new QPushButton("Select branch");

		branchLabel->setBuddy(branchSelector);

		titleLabel->setAlignment(Qt::AlignCenter);
		mainText->setAlignment(Qt::AlignCenter);
		branchLabel->setAlignment(Qt::AlignCenter);

		// populates the combo box with the branch names
		for (GITPP::BRANCH i : r.branches()) {
			branchSelector->addItem(QString::fromStdString(i.name()));
		}
	}

	// arranges the layout of the tab
	void BranchesTab::arrangeWidgets() {
		QVBoxLayout* mainLayout = new QVBoxLayout;

		mainLayout->addWidget(titleLabel);
		mainLayout->addWidget(mainText);
		mainLayout->addWidget(branchLabel);
		mainLayout->addWidget(branchSelector);
		mainLayout->addWidget(selectButton);
		setLayout(mainLayout);
	}

	// switches to the selected branch and displays message boxes to give feedback
	// on the status
	void BranchesTab::switchBranch() {
		QString selectedBranch = branchSelector->currentText();
		std::string branchName = selectedBranch.toUtf8().constData();

		try {
			r.checkout(branchName);
			switched = new QMessageBox();
			switched->setWindowTitle("Branch Switched");
			switched->setText("You have switched to the " + selectedBranch + " branch.");

			switched->exec();
		}
		catch (GITPP::EXCEPTION_CANT_FIND const& e) {
			error = new QMessageBox();
			error->setWindowTitle("Branch Not Switched");
			QString message = "An error occurred while switching to the ";
			QString message2 = " branch. Check that a local copy of the branch has been";
			QString message3 = " created on this computer and that all of your commits ";
			QString message4 = "have been pushed, or try selecting a different branch.";
			error ->setText(message + selectedBranch + message2 + message3 + message4);

			error->exec();
		}
		catch (GITPP::EXCEPTION const& e) {
			error = new QMessageBox();
			error->setWindowTitle("Branch Not Switched");
			QString message = "An error occurred while switching to the ";
			QString message2 = " branch. Check that a local copy of the branch has been";
			QString message3 = " created on this computer or try selecting a different ";
			QString message4 = "branch.";
			error ->setText(message + selectedBranch + message2 + message3 + message4);

			error->exec();
		}
	}

	// connects widgets
	void BranchesTab::makeConnections() {
		connect(selectButton, SIGNAL(clicked()), this, SLOT(switchBranch()));
	}

	INSTALL_TAB(BranchesTab, "branches");
}

#include "ll16rdjd.moc"
