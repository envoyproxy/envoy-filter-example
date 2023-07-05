#include "header_processor.h"

namespace Envoy {
namespace Http {
    ConditionProcessor::ConditionProcessor(std::string& condition_expression) {
        parseCondition(condition_expression);
    }

    bool ConditionProcessor::evaluateCondition() {
        // do map lookup
        // map[name] -> ConditionProcessor

        isTrue_ = 1;
        return isTrue_;
    }

    int ConditionProcessor::parseCondition(std::string& condition_expression) {
        // return immediately if condition has been evaluated
        // else evaluate and add to map
        return 0;
    }


    SetHeaderProcessor::SetHeaderProcessor(bool isRequest, std::string& set_header_expression) {
        isRequest_ = isRequest;

        parseOperation(set_header_expression);
    }

    int SetHeaderProcessor::parseOperation(std::string& operation) {
        int err = 0;
        // parse key

        // parse values

        // parse condition expression and call evaluate conditions on the parsed expression
        std::string expression = "";
        err = evaluateConditions(expression);
        return err;
    }

    int SetHeaderProcessor::evaluateConditions(std::string& conditions_expression) {
        // split conditions by and
        // search for negations
        // for each condition, create a new ConditionProcessor instance and evaluate the condition
        // evaluate the expression
        bool result = true;

        // set the condition based on the result
        setCondition(result);

        return 0;
    }

    int SetHeaderProcessor::evaluateOperation(RequestHeaderMap& headers) {
        bool condition = getCondition();
        const std::string& key = getKey();
        const std::vector<std::string> header_vals = getVals();

        if (!condition) {
            // do nothing because condition is false
            return 0;
        }
        
        // set header
        for (auto const& header_val : header_vals) {
            headers.setCopy(LowerCaseString(key), header_val);
        }

        return 0;
    }

} // namespace Http
} // namespace Envoy
