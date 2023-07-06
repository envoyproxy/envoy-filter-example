#include "header_processor.h"

namespace Envoy {
namespace Http {
    
    bool AclProcessor::evaluateAcl(absl::string_view acl_expression) {
        int temp = 1;
        if (acl_expression.length() > 0 || temp)
            return true;
        return true;
    }

    bool ConditionProcessor::evaluateCondition(absl::string_view condition_expression) {
        // split condition_expression into each acl, send to acl processor
        // take conjunction and possibly negation of each acl and return the result
        return true;
    }

    SetHeaderProcessor::SetHeaderProcessor(bool isRequest, absl::string_view set_header_expression) {
        isRequest_ = isRequest;
        parseOperation(set_header_expression);
    }

    int SetHeaderProcessor::parseOperation(absl::string_view operation_expression) {
        int err = 0;

        // parse key and call setKey
        std::string key = "mock_key";
        setKey(key);

        // parse values and call setVals
        std::vector<std::string> vals({"mock_val1", "mock_val2"});
        setVals(vals);

        // parse condition expression and call evaluate conditions on the parsed expression
        std::string condition_expression = "";
        evaluateCondition(condition_expression);

        return err;
    }

    int SetHeaderProcessor::evaluateCondition(absl::string_view condition_expression) {
        int err = 0;
        ConditionProcessor condition;
        setCondition(condition.evaluateCondition(condition_expression));
        return err;
    }

    int SetHeaderProcessor::evaluateOperation(RequestHeaderMap& headers) {
        int err = 0;
        bool condition_result = getCondition(); // whether the condition is true or false
        const std::string key = getKey();
        const std::vector<std::string>& header_vals = getVals();

        if (!condition_result) {
            // do nothing because condition is false
            return err;
        }
        
        // set header
        for (auto const& header_val : header_vals) {
            headers.setCopy(LowerCaseString(key), header_val);
        }

        return err;
    }

    // TODO: leaving implementation for later
    int SetRequestPath::parseOperation(absl::string_view operation_expression) {return 0;}

    int SetRequestPath::evaluateOperation(RequestHeaderMap& headers) {return 0;}

    int SetRequestPath::evaluateCondition(absl::string_view condition_expression) {return 0;}

} // namespace Http
} // namespace Envoy
