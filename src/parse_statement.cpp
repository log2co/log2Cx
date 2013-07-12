#include <cstdio>
#include "parser.h"
#include "common.h"


void TParser::ParseStatement(TSymtabNode* pRoutineId) {
    InsertLineMarker();
    
    switch (token) {
        case tcIdentifier: ParseDeclarationsOrAssignment(pRoutineId);

            break;
            // not a type but a cv-qualifier
        case tcConst:
            GetTokenAppend();
            ParseConstantDeclaration(pRoutineId);
            break;
        case tcEnum:
            GetTokenAppend();
            //            ParseEnumHeader(pRoutineId);
            break;
        case tcDo: ParseDO(pRoutineId);
            break;
        case tcWhile: ParseWHILE(pRoutineId);
            break;
        case tcIf: ParseIF(pRoutineId);
            break;
        case tcFor: ParseFOR(pRoutineId);
            break;
        case tcSwitch: ParseSWITCH(pRoutineId);
            break;
            //case tcCase:
            //case tcDefault:ParseCaseLabel(pRoutineId);
            //  break;
        case tcBreak: GetTokenAppend();
            break;
        case tcLBracket: ParseCompound(pRoutineId);
            break;
        case tcReturn: ParseRETURN(pRoutineId);
            break;
        case tcPound:
            GetTokenAppend();
            ParseDirective(pRoutineId);
            break;
    }

    if (token != tcEndOfFile) {
        Resync(tlStatementFollow, tlStatementStart);
    }
}

void TParser::ParseStatementList(TSymtabNode* pRoutineId, TTokenCode terminator) {

    do {
        ParseStatement(pRoutineId);
        while (token == tcSemicolon)GetTokenAppend();
    } while ((token != terminator) && (token != tcEndOfFile));
}

TType *TParser::ParseAssignment(const TSymtabNode *pTargetId) {

    TType *pTargetType = pTargetId->pType;
    TType *pExprType = nullptr;

    switch (token) {
        case tcEqual:
        {
            GetTokenAppend();
            pExprType = ParseExpression();
            CheckAssignmentTypeCompatible(pTargetType, pExprType,
                    errIncompatibleAssignment);
        }
            break;
        case tcPlusPlus:
            //Resync(tcPlusPlus, tlExpressionFollow);
            GetTokenAppend();
            break;
        case tcMinusMinus:
            //Resync(tcMinusMinus, tlExpressionFollow);
            GetTokenAppend();
            break;
        case tcPlusEqual:
        {
            //Resync(tcPlusEqual, tlExpressionStart);
            GetTokenAppend();
            pExprType = ParseExpression();
            CheckAssignmentTypeCompatible(pTargetType, pExprType,
                    errIncompatibleAssignment);
        }
            break;
        case tcMinusEqual:
        {
            //Resync(tcMinusEqual, tlExpressionStart);
            GetTokenAppend();
            pExprType = ParseExpression();
            CheckAssignmentTypeCompatible(pTargetType, pExprType,
                    errIncompatibleAssignment);
        }
            break;
        case tcStarEqual:
        {
            //Resync(tcStarEqual, tlExpressionStart);
            GetTokenAppend();
            pExprType = ParseExpression();
            CheckAssignmentTypeCompatible(pTargetType, pExprType,
                    errIncompatibleAssignment);
        }
            break;
        case tcForwardSlashEqual:
        {
            //Resync(tcForwardSlashEqual, tlExpressionStart);
            GetTokenAppend();
            pExprType = ParseExpression();
            CheckAssignmentTypeCompatible(pTargetType, pExprType,
                    errIncompatibleAssignment);
        }
            break;
        case tcModEqual:
        {
            //Resync(tcModEqual, tlExpressionStart);
            GetTokenAppend();
            pExprType = ParseExpression();
            CheckAssignmentTypeCompatible(pTargetType, pExprType,
                    errIncompatibleAssignment);
        }
            break;
        case tcBitLeftShiftEqual:
        {
            //Resync(tcBitLeftShiftEqual, tlExpressionStart);
            GetTokenAppend();
            pExprType = ParseExpression();
            CheckAssignmentTypeCompatible(pTargetType, pExprType,
                    errIncompatibleAssignment);
        }
            break;
        case tcBitRightShiftEqual:
        {
            //Resync(tcBitRightShiftEqual, tlExpressionStart);
            GetTokenAppend();
            pExprType = ParseExpression();
            CheckAssignmentTypeCompatible(pTargetType, pExprType,
                    errIncompatibleAssignment);
        }
            break;
        case tcBitANDEqual:
        {
            //Resync(tcBitANDEqual, tlExpressionStart);
            GetTokenAppend();
            pExprType = ParseExpression();
            CheckAssignmentTypeCompatible(pTargetType, pExprType,
                    errIncompatibleAssignment);
        }
            break;
        case tcBitXOREqual:
        {
            //Resync(tcBitXOREqual, tlExpressionStart);
            GetTokenAppend();
            pExprType = ParseExpression();
            CheckAssignmentTypeCompatible(pTargetType, pExprType,
                    errIncompatibleAssignment);
        }
            break;
        case tcBitOREqual:
        {
            //Resync(tcBitOREqual, tlExpressionStart);
            GetTokenAppend();
            pExprType = ParseExpression();
            CheckAssignmentTypeCompatible(pTargetType, pExprType,
                    errIncompatibleAssignment);
        }
            break;
        case tcComma:
        case tcSemicolon:
            break;
        case tcIdentifier:
        {
            pExprType = ParseFactor();
        }
            break;
        default:
            Error(errInvalidAssignment);
            break;
    }

    return pExprType;
}

void TParser::ParseDO(TSymtabNode* pRoutineId) {


    int breakPoint = PutLocationMarker();
    GetTokenAppend(); //do

    ParseStatementList(pRoutineId, tcWhile);

    CondGetTokenAppend(tcWhile, errMissingWHILE);
    CondGetTokenAppend(tcLParen, errMissingLeftParen);

    CheckBoolean(ParseExpression());

    CondGetTokenAppend(tcRParen, errMissingRightParen);

    FixupLocationMarker(breakPoint);
}

void TParser::ParseWHILE(TSymtabNode* pRoutineId) {

    int breakPoint = PutLocationMarker();

    GetTokenAppend(); // while

    CheckBoolean(ParseExpression());

    ParseStatement(pRoutineId);

    FixupLocationMarker(breakPoint);
}

void TParser::ParseIF(TSymtabNode* pRoutineId) {

    //--Append a placeholder location marker for where to go to if
    //--<expr> is false.  Remember the location of this placeholder
    //--so it can be fixed up below.
    int atFalseLocationMarker = PutLocationMarker();

    GetTokenAppend();
    CondGetTokenAppend(tcLParen, errMissingLeftParen);

    CheckBoolean(ParseExpression());

    CondGetTokenAppend(tcRParen, errMissingRightParen);

    ParseStatement(pRoutineId);
    while (token == tcSemicolon) GetTokenAppend();

    FixupLocationMarker(atFalseLocationMarker);
    if (token == tcElse) {
        //--Append a placeholder location marker for the token that
        //--follows the IF statement.  Remember the location of this
        //--placeholder so it can be fixed up below.
        int atFollowLocationMarker = PutLocationMarker();

        GetTokenAppend();
        ParseStatement(pRoutineId);
        while (token == tcSemicolon) GetTokenAppend();

        FixupLocationMarker(atFollowLocationMarker);
    }
}

void TParser::ParseFOR(TSymtabNode* pRoutineId) {

    TType *pControlType;

    GetTokenAppend();
    CondGetTokenAppend(tcLParen, errMissingLeftParen);

    if (token == tcIdentifier) {
        TSymtabNode *pControlId = Find(pToken->String());
        if (pControlId->defn.how != dcUndefined) {
            pControlType = pControlId->pType->Base();
        } else {
            pControlId->defn.how = dcVariable;
            pControlType = pControlId->pType = pIntegerType;
        }

        if ((pControlType != pIntegerType)
                && (pControlType != pCharType)
                && (pControlType->form != fcEnum)) {
            Error(errIncompatibleTypes);
            pControlType = pIntegerType;
        }

        icode.Put(pControlId);
    }

    if (token != tcSemicolon) {

        // declaration would go here //

        CondGetTokenAppend(tcIdentifier, errMissingIdentifier);

        //Resync(tcEqual, tlExpressionStart);
        CondGetTokenAppend(tcEqual, errMissingEqual);
        // expr 1
        CheckAssignmentTypeCompatible(pControlType, ParseExpression(),
                errIncompatibleTypes);

        CondGetTokenAppend(tcSemicolon, errMissingSemicolon);
    } else GetTokenAppend();


    if (token != tcSemicolon) {

        // expr 2
        CheckBoolean(ParseExpression());

        CondGetTokenAppend(tcSemicolon, errMissingSemicolon);
    } else GetTokenAppend();

    if (token != tcRParen) {
        // expr 3
        ParseExpression();
    }

    CondGetTokenAppend(tcRParen, errMissingRightParen);

    ParseStatement(pRoutineId);
}

void TParser::ParseSWITCH(TSymtabNode* pRoutineId) {

    GetTokenAppend();
    CondGetTokenAppend(tcLParen, errMissingLeftParen);

    TType *pExprType = ParseExpression()->Base();

    CondGetTokenAppend(tcRParen, errMissingRightParen);

    if ((pExprType != pIntegerType)
            && (pExprType != pCharType)
            && (pExprType->form != fcEnum)) {
        Error(errIncompatibleTypes);
    }

    ParseStatement(pRoutineId);

}

void TParser::ParseCaseBranch(TSymtabNode* pRoutineId, const TType *pExprType) {
    // c switch easier to parse that Pascal???
}

void TParser::ParseCaseLabel(TSymtabNode* pRoutineId, const TType *pExprType) {
    GetTokenAppend();

    bool signFlag(false);

    if (TokenIn(token, tlUnaryOps)) {
        signFlag = true;
        GetTokenAppend();
    }

    switch (token) {
        case tcIdentifier:
            if (!SearchAll(pToken->String())) {
                Error(errUndefinedIdentifier);
            }

            GetTokenAppend();
            break;
        case tcNumber:
            if (pToken->Type() != tyInteger) Error(errInvalidConstant);
            GetTokenAppend();
            break;
        case tcString:
            if (signFlag || (strlen(pToken->String()) != 3)) {
                Error(errInvalidConstant);
            }
    }

    CondGetTokenAppend(tcColon, errMissingColon);

    ParseStatementList(pRoutineId, tcBreak);
}

void TParser::ParseCompound(TSymtabNode* pRoutineId) {
    GetTokenAppend();

    ParseStatementList(pRoutineId, tcRBracket);

    CondGetTokenAppend(tcRBracket, errMissingRightBracket);

}

void TParser::ParseRETURN(TSymtabNode* pRoutineId) {
    GetTokenAppend();

    // expr 1
    CheckAssignmentTypeCompatible(pRoutineId->pType, ParseExpression(),
            errIncompatibleTypes);
}